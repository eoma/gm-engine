#pragma once

class ComponentSerializer; typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;
class ServerGameObject; typedef std::shared_ptr<ServerGameObject> ServerGameObjectPtr;
class Player; typedef std::shared_ptr<Player> PlayerPtr;

#include <DatabaseSqlite/gameobjects_repository.h>

class GameObjectManager
{
// Construction:
public:
	GameObjectManager(const GameObjectsRepositoryPtr &gameobjects_repository, const ComponentSerializerPtr &component_serializer);

// Attributes:
public: 

// Operations:
public:
	void tick(float time_elapsed);

	ServerGameObjectPtr get_gameobject(int gameobject_id);
	void get_or_load_gameobject(int gameobject_id, const std::function<void(ServerGameObjectPtr)> &func);

	void create_gameobject(const std::string &type, const std::string &name, const std::function<void(ServerGameObjectPtr)> &func);

	bool dispatch_net_event(const clan::NetGameEvent &event, const PlayerPtr &player);

// Implementation:
protected:
	void populate_gameobject(const ServerGameObjectPtr &gameobject, const GameObjectsRepository::GameObjectInfo &gameobject_info);
	ServerGameObjectPtr initialize_gameobject(const GameObjectsRepository::GameObjectInfo &gameobject_info);

	void save_dirty_gameobjects();

	void on_net_event_object_event(const clan::NetGameEvent &e, PlayerPtr player);

	GameObjectsRepositoryPtr gameobjects_repository;
	ComponentSerializerPtr component_serializer;

	std::map<int, ServerGameObjectPtr> gameobjects;

	clan::NetGameEventDispatcher_v1<PlayerPtr> netevents;

	std::multimap<int, std::function<void(ServerGameObjectPtr)>> loading_gameobjects;
};
typedef std::shared_ptr<GameObjectManager> GameObjectManagerPtr;
