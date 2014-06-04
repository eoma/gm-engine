#include "precomp.h"
#include "gameobject_manager.h"
#include "server_gameobject.h"
#include "Components/component_serializer.h"
#include "Components/property_serializer.h"
#include "../Network/definitions_netevents.h"
#include "../Network/network_receiver.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

GameObjectManager::GameObjectManager(const GameObjectsRepositoryPtr &gameobjects_repository, const ComponentSerializerPtr &component_serializer)
: gameobjects_repository(gameobjects_repository), component_serializer(component_serializer)
{
	netevents.func_event(CTS_OBJECT_EVENT).set(this, &GameObjectManager::on_net_event_object_event);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

void GameObjectManager::save_dirty_gameobjects()
{
	for(auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		(*it).second->save_dirty_properties(nullptr);
	}
}

void GameObjectManager::tick(float time_elapsed)
{
	for(auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		(*it).second->tick(time_elapsed);
	}

	static float total_time = 0;
	total_time += time_elapsed;
	if(total_time > 10.0f)
	{
		save_dirty_gameobjects();
		total_time = 0.0f;
	}
}

ServerGameObjectPtr GameObjectManager::get_gameobject(int gameobject_id)
{
	std::map<int, ServerGameObjectPtr>::iterator it = gameobjects.find(gameobject_id);
	if(it != gameobjects.end())
		return it->second;
	return nullptr;
}

void GameObjectManager::get_or_load_gameobject(int gameobject_id, const std::function<void(ServerGameObjectPtr)> &func)
{
	if(gameobject_id == 0)
		throw Exception("GameObject 0 does not exist");

	ServerGameObjectPtr gameobject = get_gameobject(gameobject_id);
	if(gameobject != nullptr)
	{
		log_event("GameObjects", "Found gameobject %1", gameobject_id);
		func(gameobject);
		return;
	}

	loading_gameobjects.insert(std::pair<int, std::function<void(ServerGameObjectPtr)>>(gameobject_id, func));
	if(loading_gameobjects.count(gameobject_id) > 1) 
	{
		log_event("GameObjects", "Already queued loading of %1", gameobject_id);
		return;
	}

	log_event("GameObjects", "Trying to load %1", gameobject_id);
	gameobjects_repository->get_info(gameobject_id, 
		[this, func, gameobject_id](GameObjectsRepository::GameObjectInfo gameobject_info)
		{
			ServerGameObjectPtr gameobject;
			if(gameobject_info.gameobject_id != 0)
			{
				gameobject = initialize_gameobject(gameobject_info);
				log_event("GameObjects", "Loaded %1 (%2 %3)", gameobject->get_id(), gameobject->get_type(), gameobject->get_name());
			}

			auto its = loading_gameobjects.equal_range(gameobject_id);
			for(auto it = its.first; it != its.second; ++it)
			{
				if(it->second != nullptr) it->second(gameobject);
			}

			loading_gameobjects.erase(gameobject_info.gameobject_id);
		}
	);
}

void GameObjectManager::create_gameobject(const std::string &type, const std::string &name, const std::function<void(ServerGameObjectPtr)> &func)
{
	gameobjects_repository->create(type, 
		[this, func, type, name](int gameobject_id) 
		{
			auto gameobject = ServerGameObjectPtr(new ServerGameObject(gameobjects_repository, gameobject_id, type, name));

			// TODO; should this function support template manager directly ?

			gameobject->save_dirty_properties(nullptr);

			gameobjects[gameobject->get_id()] = gameobject;

			gameobject->sig_initialized.invoke();

			if(func != nullptr) func(gameobject);
		}
	);	
}

bool GameObjectManager::dispatch_net_event(const NetGameEvent &event, const PlayerPtr &player)
{
	return netevents.dispatch(event, player);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

ServerGameObjectPtr GameObjectManager::initialize_gameobject(const GameObjectsRepository::GameObjectInfo &gameobject_info)
{
	auto gameobject = ServerGameObjectPtr(new ServerGameObject(gameobjects_repository, gameobject_info.gameobject_id, gameobject_info.type));
	populate_gameobject(gameobject, gameobject_info);

	for(auto it = gameobject_info.children.begin(); it != gameobject_info.children.end(); ++it)
	{
		auto child = initialize_gameobject(*it);
		gameobject->add_child(child, ServerGameObject::STORAGE_TEMPORARY);
	}

	gameobject->clearDirtyProperties();
	gameobject->track_changes();
	gameobjects[gameobject->get_id()] = gameobject;

	gameobject->sig_initialized.invoke();

	return gameobject;
}

void GameObjectManager::populate_gameobject(const ServerGameObjectPtr &gameobject, const GameObjectsRepository::GameObjectInfo &gameobject_info)
{
	for(size_t i = 0; i < gameobject_info.components.size(); ++i)
	{
		component_serializer->create_and_add_component(gameobject, gameobject_info.components[i].type, gameobject_info.components[i].name);
	}
	for(size_t i = 0; i < gameobject_info.properties.size(); ++i)
	{
		PropertySerializer::create_and_add_property(*gameobject, gameobject_info.properties[i].type_id, gameobject_info.properties[i].name, gameobject_info.properties[i].value);
	}
}

void GameObjectManager::on_net_event_object_event(const NetGameEvent &e, PlayerPtr player)
{
	int gameobject_id = e.get_argument(0);

	auto gameobject = get_gameobject(gameobject_id);
	if(gameobject)
	{
		std::string event_name = e.get_argument(1);
		NetGameEvent gameobject_event(event_name);
		for (unsigned int i = 2; i < e.get_argument_count(); i++)
			gameobject_event.add_argument(e.get_argument(i));

		auto &components = gameobject->getComponents();
		for(size_t i = 0; i < components.size(); ++i)
		{
			auto component = std::dynamic_pointer_cast<NetworkReceiver> (components[i]);
			if(component)
			{
				component->dispatch_net_event(gameobject_event, player);
			}
		}
	}
}