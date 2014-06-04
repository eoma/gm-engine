#include "precomp.h"
#include "turnsystem_realtime.h"

using namespace clan;
using namespace Totem;

TurnsystemRealtime::TurnsystemRealtime(ServerGameObjectPtr owner, const std::string &name)
: Turnsystem(owner, name)
{
}

bool TurnsystemRealtime::can_turn(const PlayerPtr &player)
{
    return std::find(has_turned.begin(), has_turned.end(), player) == has_turned.end();
}

void TurnsystemRealtime::turn(const PlayerPtr &player)
{
    has_turned.push_back(player);
}

void TurnsystemRealtime::update(const float &/*delta_time*/)
{
    has_turned.clear();
    execute_turns();
}
