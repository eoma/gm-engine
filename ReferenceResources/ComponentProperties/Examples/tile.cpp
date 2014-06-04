#include "precomp.h"
#include "tile.h"
#include "definitions_property_names.h"

using namespace Totem;
using namespace clan;

Tile::Tile(ServerGameObjectPtr owner, const std::string &name)
: Component<Tile>(name)
{
    property_tile = owner->add<int>(PROPERTY_TILE, 0);
	property_material = owner->add<int>(PROPERTY_MATERIAL, 0);
    property_obstacle = owner->add<float>(PROPERTY_OBSTACLE, 0.0f);
    property_position = owner->add<Vec3f>(PROPERTY_POSITION, Vec3f(0,0,0));

    owner->replicate_property(property_position);
    owner->replicate_property(property_obstacle);
    owner->replicate_property(property_tile);
	owner->replicate_property(property_material);

	slots.connect(property_tile.valueChanged(), clan::Callback<void(const int &, const int &)>(this, &Tile::on_tile_type_changed));
	slots.connect(property_material.valueChanged(), clan::Callback<void(const int &, const int &)>(this, &Tile::on_material_type_changed));
}

Vec3f Tile::get_position() const
{
    return property_position;
}

float Tile::get_obstacle_resistance() const
{
    return property_obstacle;
}

void Tile::on_tile_type_changed(const int &old_value, const int &new_value)
{
	//clan::log_event("Game", string_format("Tile changed to %1", new_value));
}

void Tile::on_material_type_changed(const int &old_value, const int &new_value)
{
	//clan::log_event("Game", string_format("Material changed to %1", new_value));
}
