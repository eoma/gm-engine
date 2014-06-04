#pragma once

#include "definitions_component_names.h"
#include "TurnSystem/turnlistener.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/Zone/zone_manager.h>
#include <ServerEngine/GameWorld/Components/replicated_component.h>

class Movement : public Totem::Component<Movement>, public ReplicatedComponent, public TurnListener
{
public:
	Movement(ServerGameObjectPtr owner, const ZoneManagerPtr &zone_manager, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_MOVEMENT; }

protected:
    virtual void on_turn() override;

	ServerGameObjectPtr owner;
    ZoneManagerPtr zone_manager;

	Totem::Property<clan::Vec3f> property_position;
    clan::Vec2f destination_direction;
	
	void on_set_destination(const clan::Vec2i &delta_move);

	clan::CallbackContainer slots;
};
