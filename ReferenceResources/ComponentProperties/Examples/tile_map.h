#pragma once

#include "definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/gameobject_manager.h>
#include <ServerEngine/GameWorld/Zone/zone_manager.h>
#include <ServerEngine/GameWorld/Components/replicated_component.h>
#include <SquidLibCpp/Grid/Utility/map.h>

class TileMap : public Totem::Component<TileMap>, public ReplicatedComponent
{
// Construction:
public:
	TileMap(ServerGameObjectPtr owner, const GameObjectManagerPtr &gameobject_manager, const ZoneManagerPtr &zone_manager, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_TILEMAP; }

// Attributes:
public:
	bool use_fov() const;
	float fov_range() const;
	bool is_dirty() const { return resistance_map_dirty; }

	ServerGameObjectPtr get_gameobject_at(const clan::Vec3f &pos);

// Operations:
public:
	bool can_move_to(clan::Vec3f position);

	SquidLibCpp::MapPtr calculate_fov(int pos_x, int pos_y, float radius);
	SquidLibCpp::MapPtr calculate_fov(const clan::Vec3f &pos, float radius);

// Implementation:
private:
    void calculate_map();

	void on_child_added(const ServerGameObjectPtr &child, bool moved);
	void on_child_removed(const ServerGameObjectPtr &child, bool moved);
	void on_child_changed(const float &old_value, const float &new_value);

	SquidLibCpp::MapPtr resistance_map;
	bool resistance_map_dirty;

	Totem::Property<float> property_movement_obstacle_limit;

	ServerGameObjectPtr owner;
    GameObjectManagerPtr gameobject_manager;
	ZoneManagerPtr zone_manager;

	clan::CallbackContainer slots;
};
typedef std::shared_ptr<TileMap> TileMapPtr;
