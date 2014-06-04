#include "precomp.h"
#include "server_gameobject.h"
#include "player.h"
#include "Components/property_serializer.h"
#include "../Network/definitions_netevents.h"
#include <DatabaseSqlite/gameobjects_repository.h>

using namespace Totem;
using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

// Used when creating gameobject from database
ServerGameObject::ServerGameObject(const GameObjectsRepositoryPtr &gameobjects_repository, int id, const std::string &type)
: Totem::ComponentContainer<>(), Totem::PropertyContainer<>(), destroyed(false), id(id), type(type), gameobjects_repository(gameobjects_repository)
{
	property_name = add<std::string>("Name", "");
	property_replicate = add<bool>("Replicate", true);
}

// Used when creating gameobject from scratch
ServerGameObject::ServerGameObject(const GameObjectsRepositoryPtr &gameobjects_repository, int id, const std::string &type, const std::string &name)
: Totem::ComponentContainer<>(), Totem::PropertyContainer<>(), destroyed(false), id(id), type(type), gameobjects_repository(gameobjects_repository)
{
	track_changes();

	property_name = add<std::string>("Name", name);
	property_replicate = add<bool>("Replicate", true);
}

ServerGameObject::~ServerGameObject()
{
//	log_event("Debug", "Destroying gameobject %1 %2", get_type(), get_name());
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int ServerGameObject::get_id() const
{ 
	return id; 
}

ServerGameObjectPtr ServerGameObject::get_parent() const
{ 
	return parent.lock();
}

const std::string &ServerGameObject::get_type() const 
{
	return type;
}

const std::string &ServerGameObject::get_name() const
{ 
	return property_name.get();
}

bool ServerGameObject::is_destroyed() const 
{ 
	return destroyed; 
}

ServerGameObjectPtr ServerGameObject::find_child(int id)
{
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		auto object = *it;
		if(object->get_id() == id)
			return object;
	}

	return nullptr;
}

ServerGameObjectPtr ServerGameObject::find_child_by_type(const std::string &type)
{
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		auto object = *it;
		if(object->get_type() == type)
			return object;
	}

	return nullptr;
}

std::list<ServerGameObjectPtr> ServerGameObject::get_children()
{
	return children;
}

std::list<ServerGameObjectPtr> ServerGameObject::get_children_by_type(const std::string &type)
{
	std::list<ServerGameObjectPtr> results;
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		auto object = *it;
		if(object->get_type() == type)
			results.push_back(object);
	}
	return results;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void ServerGameObject::tick(float time_elapsed)
{
	updateComponents(time_elapsed);
	updateProperties();
}

void ServerGameObject::destroy() 
{ 
	destroyed = true; 
}

void ServerGameObject::add_child(const ServerGameObjectPtr &gameobject, ServerGameObject::StorageMode mode)
{
	children.push_back(gameobject);

	auto previous_parent = gameobject->get_parent();

	if(previous_parent != nullptr)
	{
		// Remove from previous parent
		auto it = std::find(previous_parent->children.begin(), previous_parent->children.end(), gameobject);
		if (it != previous_parent->children.end())
		{
			previous_parent->children.erase(it);
		}

		previous_parent->sig_child_removed.invoke(gameobject, true);
	}

	gameobject->set_parent(shared_from_this());

	if(mode == STORAGE_PERMANENT)
		gameobjects_repository->set_parent_id(gameobject->get_id(), get_id(), nullptr);

	sig_child_added.invoke(gameobject, previous_parent != nullptr);
}

bool ServerGameObject::remove_child(const ServerGameObjectPtr &gameobject, ServerGameObject::StorageMode mode)
{
	auto it = std::find(children.begin(), children.end(), gameobject);
	if (it != children.end())
	{
		children.erase(it);

		gameobject->clear_parent();

		if(mode == STORAGE_PERMANENT)
			gameobjects_repository->set_parent_id(gameobject->get_id(), 0, nullptr);

		sig_child_removed.invoke(gameobject, false);

		return true;
	}

	return false;
}

void ServerGameObject::send_event(const NetGameEvent &event, const PlayerPtr &player)
{
	NetGameEvent marshalled_event(STC_OBJECT_EVENT, id, event.get_name());
	for (unsigned int i = 0; i < event.get_argument_count(); i++)
		marshalled_event.add_argument(event.get_argument(i));

	player->send_event(marshalled_event);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void ServerGameObject::set_parent(const ServerGameObjectWeakPtr &parent)
{
	this->parent = parent;
}

void ServerGameObject::clear_parent()
{
    parent.reset();
}

void ServerGameObject::track_changes()
{
	slots.connect(propertyAdded(), clan::Callback<void(IPropertyPtr)>(this, &ServerGameObject::on_property_added));
	slots.connect(propertyRemoved(), clan::Callback<void(IPropertyPtr)>(this, &ServerGameObject::on_property_removed));
	slots.connect(componentAdded(), clan::Callback<void(IComponentPtr)>(this, &ServerGameObject::on_component_added));
	slots.connect(componentRemoved(), clan::Callback<void(IComponentPtr)>(this, &ServerGameObject::on_component_removed));
}

void ServerGameObject::on_property_added(IPropertyPtr property)
{
	PropertySerializer::PropertyType type = PropertySerializer::get_property_type(property);
	GameObjectsRepository::PropertyType db_type = static_cast<GameObjectsRepository::PropertyType>(type);

	gameobjects_repository->add_property(id, db_type, property->getName(), PropertySerializer::property_value_to_string(property), nullptr);
	property->clearDirty();
}

void ServerGameObject::on_property_removed(IPropertyPtr property)
{
	gameobjects_repository->remove_property(id, property->getName(), nullptr);
}

void ServerGameObject::on_component_added(IComponentPtr component)
{
	gameobjects_repository->add_component(id, component->getType(), component->getName(), nullptr);
}

void ServerGameObject::on_component_removed(IComponentPtr component)
{
	gameobjects_repository->remove_component(id, component->getType(), component->getName(), nullptr);
}

void ServerGameObject::save_dirty_properties(const std::function<void()> &func) 
{
	std::vector<GameObjectsRepository::GameObjectDirtyPropertyInfo> dirty_properties;

	auto &components = getComponents();
	for(auto cit = components.begin(); cit != components.end(); ++cit)
	{
		auto component = (*cit);
		auto &properties = component->getProperties();
		for(auto it = properties.begin(); it != properties.end(); ++it)
		{
			auto property = (*it).second;
			if(property->isDirty())
			{
				GameObjectsRepository::GameObjectDirtyPropertyInfo database_property;
				database_property.gameobject_id = id;
				database_property.component_name = component->getName();
				database_property.property_name = property->getName();
				database_property.property_value = PropertySerializer::property_value_to_string(property);
				dirty_properties.push_back(database_property);
			}
		}
	}

	auto &properties = getProperties();
	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		auto property = (*it).second;
		if(property->isDirty())
		{
			GameObjectsRepository::GameObjectDirtyPropertyInfo database_property;
			database_property.gameobject_id = id;
			database_property.property_name = property->getName();
			database_property.property_value = PropertySerializer::property_value_to_string(property);
			dirty_properties.push_back(database_property);

//            log_event("Database", "Serializing %1 %2", database_property.property_name, database_property.property_value);
		}
	}

	if(!dirty_properties.empty()) 
	{
//		log_event("Database", "Serializing %1 properties", dirty_properties.size());
		gameobjects_repository->save_dirty_properties(dirty_properties, func);
		clearDirtyProperties();
	}
}
