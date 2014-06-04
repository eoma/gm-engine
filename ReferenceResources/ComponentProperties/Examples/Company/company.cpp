#include "precomp.h"
#include "company.h"
#include "../definitions_property_names.h"

using namespace clan;
using namespace Totem;

// TODO, this is really a PlayerCharacter

Company::Company(ServerGameObjectPtr owner, const ZoneManagerPtr &zone_manager, const std::string &name)
: owner(owner), zone_manager(zone_manager), Component<Company>(name)
{
	property_current_zone_id = owner->add<int>(PROPERTY_CURRENT_ZONE_ID, 0);
}

int Company::get_current_zone_id() const
{ 
	return property_current_zone_id;
}

PlayerPtr Company::get_player() const
{
	return player;
}

void Company::set_player(const PlayerPtr &player)
{
	this->player = player;
}

void Company::set_current_zone_id(int zone_id)
{
	property_current_zone_id = zone_id;
}

ZonePtr Company::get_current_zone() const
{
    return zone_manager->find_zone(property_current_zone_id);
}
