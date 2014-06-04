#pragma once

#include "turnsystem.h"
#include "../definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/player.h>

class TurnsystemRealtime : public Turnsystem
{
// Construction:
public:
    TurnsystemRealtime(ServerGameObjectPtr owner, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_TURNSYSTEM_REALTIME; }

// Attributes:
public:
    virtual bool can_turn(const PlayerPtr &player) override;

// Operations:
public:
    virtual void turn(const PlayerPtr &player) override;

    virtual void update(const float &delta_time) override;

// Implementation:
private:
    std::list<PlayerPtr> has_turned;
};
typedef std::shared_ptr<TurnsystemRealtime> TurnsystemRealtimePtr;
