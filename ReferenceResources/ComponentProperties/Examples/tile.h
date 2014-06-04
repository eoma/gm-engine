#pragma once

#include "definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/Components/replicated_component.h>

class Tile : public Totem::Component<Tile>, public ReplicatedComponent
{
public:
	Tile(ServerGameObjectPtr owner, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_TILE; }

// Attributes:
public:
    clan::Vec3f get_position() const;

    float get_obstacle_resistance() const;

// Implementation:
protected:
    Totem::Property<int> property_tile;
	Totem::Property<int> property_material;
    Totem::Property<float> property_obstacle;
    Totem::Property<clan::Vec3f> property_position;

	clan::CallbackContainer slots;
	void on_tile_type_changed(const int &old_value, const int &new_value);
	void on_material_type_changed(const int &old_value, const int &new_value);
};
typedef std::shared_ptr<Tile> TilePtr;
