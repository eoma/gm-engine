#pragma once

#include <ServerEngine/GameWorld/Zone/zone_manager.h>
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/Components/component_serializer.h>

class GameComponentSerializer
{
public:
	GameComponentSerializer(const ComponentSerializerPtr &component_serializer, const ZoneManagerPtr &zone_manager, const GameObjectManagerPtr &gameobject_manager);

	void create_and_add_component(const ServerGameObjectPtr &owner, const std::string &type, const std::string &name);

private:
	GameObjectManagerPtr gameobject_manager;
	ZoneManagerPtr zone_manager;
};

typedef std::shared_ptr<GameComponentSerializer> GameComponentSerializerPtr;
