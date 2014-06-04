#include "precomp.h"
#include "game_component_serializer.h"
#include "transform.h"
#include "tile.h"
#include "movement.h"
#include "world.h"
#include "world_tile.h"
#include "Company/company.h"
#include "Company/company_info.h"
#include "tile_map.h"
#include "TurnSystem/turnsystem_discrete.h"
#include "TurnSystem/turnsystem_realtime.h"
#include "Regions/region.h"

using namespace Totem;
using namespace clan;

GameComponentSerializer::GameComponentSerializer(const ComponentSerializerPtr &component_serializer, const ZoneManagerPtr &zone_manager, const GameObjectManagerPtr &gameobject_manager)
: zone_manager(zone_manager), gameobject_manager(gameobject_manager)
{
	component_serializer->sig_create_component.connect(
		clan::Callback<void(const ServerGameObjectPtr &, const std::string &, const std::string &)>(
		this, &GameComponentSerializer::create_and_add_component));
}

void GameComponentSerializer::create_and_add_component(const ServerGameObjectPtr &owner, const std::string &type, const std::string &name)
{
	if(type == Transform::getStaticType())
	{
		owner->addComponent(std::make_shared<Transform>(owner, name));
	}
	else if(type == Tile::getStaticType())
	{
		owner->addComponent(std::make_shared<Tile>(owner, name));
	}
	else if(type == Movement::getStaticType())
	{
		owner->addComponent(std::make_shared<Movement>(owner, zone_manager, name));
	}
	else if(type == World::getStaticType())
	{
		owner->addComponent(std::make_shared<World>(owner, name));
	}
	else if(type == WorldTile::getStaticType())
	{
		owner->addComponent(std::make_shared<WorldTile>(owner, name));
	}
	else if(type == Company::getStaticType())
	{
		owner->addComponent(std::make_shared<Company>(owner, zone_manager, name));
	}
	else if (type == CompanyInfo::getStaticType())
	{
		owner->addComponent(std::make_shared<CompanyInfo>(owner, name));
	}
    else if(type == TileMap::getStaticType())
    {
        owner->addComponent(std::make_shared<TileMap>(owner, gameobject_manager, zone_manager, name));
    }
    else if(type == TurnsystemDiscrete::getStaticType())
    {
        owner->addComponent(std::make_shared<TurnsystemDiscrete>(owner, name));
    }
    else if(type == TurnsystemRealtime::getStaticType())
    {
        owner->addComponent(std::make_shared<TurnsystemRealtime>(owner, name));
    }
	else if(type == Region::getStaticType())
    {
		owner->addComponent(std::make_shared<Region>(owner, name));
    }
	else
	{
		throw Exception(string_format("GameComponentSerializer::create_and_add_component - Unknown component type %1", type));
	}
}
