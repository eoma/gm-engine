#include "precomp.h"
#include "turnsystem.h"
#include "turnlistener.h"
#include "../definitions_turn_groups.h"
#include "../Company/company.h"

using namespace clan;
using namespace Totem;

Turnsystem::Turnsystem(ServerGameObjectPtr owner, const std::string &name)
: Component<Turnsystem>(name), owner(owner)
{
	owner->sig_child_added.connect(clan::Callback<void(const ServerGameObjectPtr &, bool)>(this, &Turnsystem::on_child_added));
	owner->sig_child_removed.connect(clan::Callback<void(const ServerGameObjectPtr &, bool)>(this, &Turnsystem::on_child_removed));
}

void Turnsystem::on_child_added(const ServerGameObjectPtr &gameobject, bool moved)
{
    auto is_a_player = gameobject->hasComponent<Company>();
    auto components = gameobject->getComponents();
    for(auto it = components.begin(); it != components.end(); ++it)
    {
        TurnListenerPtr turnlistener = std::dynamic_pointer_cast<TurnListener>(*it);
        if(turnlistener)
        {
//            log_event("Debug", string_format("Adding %1 turnlistener for %2", turnlistener->get_group(), gameobject->get_name()));
            add_listener(turnlistener, is_a_player);
        }
    }
}

void Turnsystem::on_child_removed(const ServerGameObjectPtr &gameobject, bool moved)
{
    auto components = gameobject->getComponents();
    for(auto it = components.begin(); it != components.end(); ++it)
    {
        TurnListenerPtr turnlistener = std::dynamic_pointer_cast<TurnListener>(*it);
        if(turnlistener)
        {
//            log_event("Debug", string_format("Removing %1 turnlistener for %2", turnlistener->get_group(), gameobject->get_name()));
            remove_listener(turnlistener);
        }
    }
}

void Turnsystem::add_listener(const TurnListenerPtr &listener, bool is_a_player)
{
    listeners.push_back(Listener(listener, is_a_player));
}

void Turnsystem::remove_listener(const TurnListenerPtr &listener)
{
    for(auto it = listeners.begin(); it != listeners.end(); ++it)
    {
        if((*it).listener == listener)
        {
            listeners.erase(it);
            return;
        }
    }
}

void Turnsystem::execute_turns()
{
    execute_turn(PLAYERS_ONLY, TURN_GROUP_ACTION);
    execute_turn(ALL, TURN_GROUP_ENVIRONMENT);
    execute_turn(NPCS_ONLY, TURN_GROUP_ACTION);
    execute_turn(ALL, TURN_GROUP_EFFECTS);
}

void Turnsystem::execute_turn(Turnsystem::TargetListener target, std::string group)
{
//    log_event("Debug", string_format("Executing %1 turn %2 for ", group, owner->get_type()));
    for(auto it = listeners.begin(); it != listeners.end(); ++it)
    {
        Listener &listener = (*it);
        if((target == PLAYERS_ONLY && listener.is_a_player) || (target == NPCS_ONLY && !listener.is_a_player) || (target == ALL))
            if(listener.listener->get_group() == group)
                listener.listener->on_turn();
    }
}
