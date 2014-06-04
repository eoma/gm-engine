#include "precomp.h"
#include "movement.h"
#include "definitions_property_names.h"
#include "definitions_turn_groups.h"
#include "tile_map.h"
#include "../definitions_game_events.h"
#include <ServerEngine/GameWorld/Zone/zone.h>

using namespace Totem;
using namespace clan;

Movement::Movement(ServerGameObjectPtr owner, const ZoneManagerPtr &zone_manager, const std::string &name)
: Component<Movement>(name),
  TurnListener(TURN_GROUP_ACTION),
  owner(owner),
  zone_manager(zone_manager)
{
	property_position = owner->add<Vec3f>(PROPERTY_POSITION, Vec3f(0,0,0));

    // TODO, we could use a property_destination_position instead of an event
	slots.connect(owner->registerToSignal<const Vec2i &>(GAME_SET_DESTINATION), 
		clan::Callback<void(const clan::Vec2i &)>(this, &Movement::on_set_destination));

    owner->replicate_property(property_position);
}

void Movement::on_turn()
{
    if(destination_direction.x || destination_direction.y)
    {
        log_event("Debug", string_format("Moving %1 %2", destination_direction.x, destination_direction.y));

        auto destination_position = property_position.get() + Vec3f(destination_direction.x, destination_direction.y, 0.0f);

        log_event("Debug", "Setting new position");

        ZonePtr zone = zone_manager->find_zone_with_gameobject(owner);
		if(zone != nullptr)
		{
			auto tilemap = zone->get_container_gameobject()->getComponent<TileMap>();
			if(tilemap->can_move_to(destination_position))
			{
		        property_position = destination_position;

		        log_event("Debug", string_format("Triggering visibility check"));
				zone->trigger_visibility_check(owner);
			    log_event("Debug", string_format("Triggered visibility check"));
			}
			else
			{
				ServerGameObjectPtr gameobject = tilemap->get_gameobject_at(destination_position);
				if(gameobject != nullptr)
				{
			        gameobject->invoke<ServerGameObjectPtr>(GAME_BUMP_ACTION, owner, false);
				}
			}
		}

		destination_direction = Vec2i(0,0);
    }
}

void Movement::on_set_destination(const clan::Vec2i &delta_move)
{
    // TODO, make sure delta_move is only -1,0 or 1 in both x and y

    destination_direction = Vec2i(delta_move.x, delta_move.y);
}
