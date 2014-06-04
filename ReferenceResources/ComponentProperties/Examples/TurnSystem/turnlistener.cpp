#include "precomp.h"
#include "turnlistener.h"

using namespace clan;
using namespace Totem;

TurnListener::TurnListener(const std::string &group)
: group(group)
{
}

std::string TurnListener::get_group() const
{
    return group;
}
