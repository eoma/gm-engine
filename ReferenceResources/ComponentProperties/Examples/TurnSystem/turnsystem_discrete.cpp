#include "precomp.h"
#include "turnsystem_discrete.h"

using namespace clan;
using namespace Totem;

TurnsystemDiscrete::TurnsystemDiscrete(ServerGameObjectPtr owner, const std::string &name)
: Turnsystem(owner, name), player_has_turned(false)
{
}

bool TurnsystemDiscrete::can_turn(const PlayerPtr &player)
{
    return !player_has_turned;
}

void TurnsystemDiscrete::turn(const PlayerPtr &player)
{
    player_has_turned = true;
}

void TurnsystemDiscrete::update(const float &/*delta_time*/)
{
    if(player_has_turned)
    {
        execute_turns();
        player_has_turned = false;
    }
}
