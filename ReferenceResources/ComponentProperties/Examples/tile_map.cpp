#include "precomp.h"
#include "tile_map.h"
#include "tile.h"
#include "definitions_property_names.h"

using namespace clan;
using namespace Totem;

TileMap::TileMap(ServerGameObjectPtr owner, const GameObjectManagerPtr &gameobject_manager, const ZoneManagerPtr &zone_manager, const std::string &name)
: Component<TileMap>(name), owner(owner), gameobject_manager(gameobject_manager), zone_manager(zone_manager)
{
	property_movement_obstacle_limit = owner->add<float>(PROPERTY_MOVEMENT_OBSTACLE_LIMIT, 0.2f);

	//slots.connect(owner->sig_initialized, this, &TileMap::on_owner_initialized);
	slots.connect(owner->sig_child_added, clan::Callback<void(const ServerGameObjectPtr &, bool)>(this, &TileMap::on_child_added));
	slots.connect(owner->sig_child_removed, clan::Callback<void(const ServerGameObjectPtr &, bool)>(this, &TileMap::on_child_removed));
}

ServerGameObjectPtr TileMap::get_gameobject_at(const clan::Vec3f &pos)
{
	// TODO: We should first look for any dynamic objects at this position, then tiles (like doors, etc).
	for(auto &gameobject : owner->get_children())
	{
		if(gameobject->hasProperty(PROPERTY_POSITION))
		{
			if(gameobject->get<Vec3f>(PROPERTY_POSITION) == pos)
				return gameobject;
		}
	}
	return nullptr;
}

bool TileMap::can_move_to(clan::Vec3f position)
{
	if(resistance_map_dirty)
	{
		calculate_map();
		resistance_map_dirty = false;
	}

	return resistance_map->get(position) < property_movement_obstacle_limit;
}

void TileMap::calculate_map()
{
	std::list<TilePtr> tiles = owner->get_children_components<Tile>();
	if (tiles.size()) 
	{
		unsigned int min_x = -1;
		unsigned int min_y = -1;
		unsigned int max_x = 0;
		unsigned int max_y = 0;
		for (auto &tile : tiles) 
		{
			auto pos = tile->get_position();

			if (pos.x < min_x)
				min_x = (unsigned int)pos.x;
			if (pos.y < min_y)
				min_y = (unsigned int)pos.y;
			if (pos.x > max_x)
				max_x = (unsigned int)pos.x;
			if (pos.y > max_y)
				max_y = (unsigned int)pos.y;
		}

		int width = max_x - min_x + 1;
		int height = max_y - min_y + 1;

		resistance_map = SquidLibCpp::MapPtr(new SquidLibCpp::Map(width, height));

		for (auto &tile : tiles)
		{
			auto pos = tile->get_position();

			if(tile->get_obstacle_resistance() > 0)
			{
				resistance_map->set(pos, tile->get_obstacle_resistance());
			}
		}

		log_event("Debug", string_format("Initialized map of [%1,%2] - [%3, %4]", min_x, min_y, max_x, max_y));
	}
}

void TileMap::on_child_added(const ServerGameObjectPtr &child, bool /*moved*/)
{
	if(child->hasComponent<Tile>())
	{
		auto tile = child->getComponent<Tile>();
		resistance_map_dirty = true;

		if(child->hasProperty(PROPERTY_OBSTACLE))
			slots.connect(child->get<float>(PROPERTY_OBSTACLE).valueChanged(), 
				clan::Callback<void(const float &, const float &)>(this, &TileMap::on_child_changed));
	}
}

void TileMap::on_child_changed(const float &/*old_value*/, const float &/*new_value*/)
{
	resistance_map_dirty = true;
}

void TileMap::on_child_removed(const ServerGameObjectPtr &child, bool /*moved*/)
{
	if(child->hasComponent<Tile>())
	{
		// TODO, need to remove valueChanged slot on obstacle!

		resistance_map_dirty = true;
	}
}
