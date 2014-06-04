#include "precomp.h"
#include "replicated_container.h"
#include "replicated_component.h"
#include "property_serializer.h"
#include "../server_gameobject.h"
#include "../player.h"
#include "../../Network/definitions_netevents.h"

using namespace Totem;
using namespace clan;

// TODO; Lacking support for component properties (see commented code further down)

/////////////////////////////////////////////////////////////////////////////
// Construction:

ReplicatedContainer::ReplicatedContainer(Player *player, const ComponentSerializerPtr &component_serializer)
: player(player), component_serializer(component_serializer)
{
}

ReplicatedContainer::~ReplicatedContainer()
{
//	log_event("Network", "Shutting down replicating container %1 for player %2", id, player->get_user_id());

	objects.clear();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:


/////////////////////////////////////////////////////////////////////////////
// Operations:

void ReplicatedContainer::add_gameobject(const ServerGameObjectPtr &gameobject)
{
	if(std::find(objects.begin(), objects.end(), gameobject) != objects.end())
		return;	// GameObject already tracked for replication

	objects.push_back(gameobject);
	replicate_new_gameobject(gameobject);
}

void ReplicatedContainer::remove_gameobject(const ServerGameObjectPtr &gameobject)
{
	auto it = std::find(objects.begin(), objects.end(), gameobject);
	if (it == objects.end())
		return;

	objects.erase(it);

	log_event("Network", "Destroying gameobject %1 for player %2", gameobject->get_name(), player->get_user_id());

	int parent_id = 0;
	if(gameobject->get_parent() != nullptr)
		parent_id = gameobject->get_parent()->get_id();

	NetGameEvent destroyEvent(STC_OBJECT_DESTROY, gameobject->get_id(), parent_id);
	player->send_event(destroyEvent);
}

void ReplicatedContainer::attach_gameobject(const ServerGameObjectPtr &gameobject)
{
	if(std::find(objects.begin(), objects.end(), gameobject) == objects.end())
	{
		objects.push_back(gameobject);
		replicate_new_gameobject(gameobject);
	}
	else 
	{
		replicate_existing_gameobject(gameobject);
	}
}

void ReplicatedContainer::detach_gameobject(const ServerGameObjectPtr &gameobject)
{
	int parent_id = 0;
	if(gameobject->get_parent() != nullptr)
		parent_id = gameobject->get_parent()->get_id();

	NetGameEvent detachEvent(STC_OBJECT_DETACH, gameobject->get_id(), parent_id);
	player->send_event(detachEvent);
}

void ReplicatedContainer::clear_dirty_gameobjects()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		auto gameobject = objects[i];
		gameobject->clear_dirty_tracked_properties();

        /* // Enable this to support component-properties
		auto &components = gameobject->getComponents();
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			auto comp = (*it);
			auto replicated_component = std::dynamic_pointer_cast<ReplicatedComponent>(comp);
			if(replicated_component)
			{
				replicated_component->clear_dirty_properties();
			}
		}*/
	}
}

void ReplicatedContainer::sync_dirty_gameobjects()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		auto gameobject = objects[i];

		NetGameEvent create_event(STC_OBJECT_UPDATE);
		create_event.add_argument(gameobject->get_id());
		bool has_dirty_properties = false;

		auto dirty_properties = gameobject->get_dirty_tracked_properties();
		if(dirty_properties.size() > 0)
		{
			has_dirty_properties = true;

			unsigned int dirty_properties_count = (unsigned int)dirty_properties.size();
			create_event.add_argument(dirty_properties_count);
			for(size_t j = 0; j < dirty_properties_count; j++)
			{
				create_event.add_argument(PropertySerializer::get_property_type(dirty_properties[j]));
				create_event.add_argument(dirty_properties[j]->getName());
				create_event.add_argument(PropertySerializer::property_value_to_string(dirty_properties[j]));
			}

			log_event("Network", "Replicated updates on %1\\%2 for player %3", gameobject->get_parent()->get_name(), gameobject->get_name(), player->get_user_id());
		}

        /* // Enable this to enable component-properties, and add some event argument which component it belongs to
		auto &components = gameobject->getComponents();
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			auto comp = (*it);
			auto replicated_component = std::dynamic_pointer_cast<ReplicatedComponent>(comp);
			if(replicated_component)
			{
				auto dirty_properties = replicated_component->get_dirty_properties();
				if(dirty_properties.size() > 0)
				{
					has_dirty_properties = true;

					unsigned int dirty_properties_count = (unsigned int)dirty_properties.size();
					create_event.add_argument(dirty_properties_count);
					for(size_t i = 0; i < dirty_properties_count; ++i)
					{
						create_event.add_argument(PropertySerializer::get_property_type(dirty_properties[i]));
						create_event.add_argument(dirty_properties[i]->getName());
						create_event.add_argument(PropertySerializer::property_value_to_string(dirty_properties[i]));
					}

					log_event("Network", "Replicated updated component %1 on %3\\%2 for player %4", comp->getType(), gameobject->get_name(), id, player->get_user_id());
				}
			}
		}*/

		if(has_dirty_properties) {
			player->send_event(create_event);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void ReplicatedContainer::replicate_existing_gameobject(const ServerGameObjectPtr &gameobject)
{
	int parent_id = 0;
	if(gameobject->get_parent() != nullptr)
		parent_id = gameobject->get_parent()->get_id();

	NetGameEvent create_event(STC_OBJECT_ATTACH);
	create_event.add_argument(gameobject->get_id());
	create_event.add_argument(parent_id);

	player->send_event(create_event);
}

void ReplicatedContainer::replicate_new_gameobject(const ServerGameObjectPtr &gameobject)
{
	int parent_id = 0;
	if(gameobject->get_parent() != nullptr)
		parent_id = gameobject->get_parent()->get_id();

	NetGameEvent create_event(STC_OBJECT_CREATE);
	create_event.add_argument(gameobject->get_id());
    create_event.add_argument(parent_id);
	create_event.add_argument(gameobject->get_type());
	create_event.add_argument(gameobject->get_name());

//	log_event("Network", "Replicated gameobject %2\\%1 for player %3", gameobject->get_name(), id, player->get_user_id());

	auto &components = gameobject->getComponents();
	unsigned int replicateComponentCount = 0;
	for(auto it = components.begin(); it != components.end(); ++it)
	{
		if(std::dynamic_pointer_cast<ReplicatedComponent>(*it))
		{
			replicateComponentCount++;
		}
	}
	create_event.add_argument(replicateComponentCount);

	for(auto it = components.begin(); it != components.end(); ++it)
	{
		auto comp = (*it);
		auto replicated_component = std::dynamic_pointer_cast<ReplicatedComponent>(comp);
		if(replicated_component)
		{
			create_event.add_argument(comp->getType());
			create_event.add_argument(comp->getName());

/*			// Enable this to enable component-properties, and add some event argument which component it belongs to
			auto properties = replicated_component->get_tracked_properties();
			unsigned int properties_count = (unsigned int)properties.size();
			create_event.add_argument(properties_count);
			for(size_t i = 0; i < properties_count; ++i)
			{
				create_event.add_argument(PropertySerializer::get_property_type(properties[i]));
				create_event.add_argument(properties[i]->getName());
				create_event.add_argument(PropertySerializer::property_value_to_string(properties[i]));
			}

			replicated_component->clear_dirty_properties();
*/
//			log_event("Network", "Replicated component %1 on %3\\%2 for player %4", comp->getType(), gameobject->get_name(), name, player->get_user_id());
		}
	}

	auto tracked_properties = gameobject->get_tracked_properties();
	unsigned int tracked_properties_count = (unsigned int)tracked_properties.size();
	create_event.add_argument(tracked_properties_count);
	for(size_t i = 0; i < tracked_properties_count; ++i)
	{
		create_event.add_argument(PropertySerializer::get_property_type(tracked_properties[i]));
		create_event.add_argument(tracked_properties[i]->getName());
		create_event.add_argument(PropertySerializer::property_value_to_string(tracked_properties[i]));
	}
	gameobject->clear_dirty_tracked_properties();

	player->send_event(create_event);
}
