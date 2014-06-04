#pragma once

#include "../definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/player.h>
#include <ServerEngine/GameWorld/Components/replicated_component.h>
#include <ServerEngine/GameWorld/Zone/zone_manager.h>

class Company : public Totem::Component<Company>, public ReplicatedComponent
{
// Construction:
public:
	Company(ServerGameObjectPtr owner, const ZoneManagerPtr &zone_manager, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_COMPANY; }

// Attributes:
public:
	int get_current_zone_id() const;
    ZonePtr get_current_zone() const;
	PlayerPtr get_player() const;

// Operations:
public:
	void set_player(const PlayerPtr &player);

	void set_current_zone_id(int zone_id);

// Implementation:
private:
	Totem::Property<int> property_current_zone_id;

    ZoneManagerPtr zone_manager;

	PlayerPtr player;
    ServerGameObjectPtr owner;
};
typedef std::shared_ptr<Company> CompanyPtr;
