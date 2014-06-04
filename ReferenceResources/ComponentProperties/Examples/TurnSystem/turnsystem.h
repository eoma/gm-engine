#pragma once

#include "../definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/player.h>

class TurnListener; typedef std::shared_ptr<TurnListener> TurnListenerPtr;

class Turnsystem : public Totem::Component<Turnsystem>
{
// Construction:
public:
    Turnsystem(ServerGameObjectPtr owner, const std::string &name = std::string());

// Attributes:
public:
    virtual bool can_turn(const PlayerPtr &player) = 0;

// Operations:
public:
    void add_listener(const TurnListenerPtr &listener, bool is_a_player);
    void remove_listener(const TurnListenerPtr &listener);

    virtual void turn(const PlayerPtr &player) = 0;

    void execute_turns();

// Implementation:
private:
    ServerGameObjectPtr owner;

    enum TargetListener
    {
        PLAYERS_ONLY,
        NPCS_ONLY,
        ALL
    };

    struct Listener
    {
        Listener(TurnListenerPtr listener, bool is_a_player) : listener(listener), is_a_player(is_a_player) {}
        TurnListenerPtr listener;
        bool is_a_player;
    };

    std::list<Listener> listeners;

    void on_child_added(const ServerGameObjectPtr &gameobject, bool moved);
    void on_child_removed(const ServerGameObjectPtr &gameobject, bool moved);

    void execute_turn(TargetListener target, std::string group);
};
typedef std::shared_ptr<Turnsystem> TurnsystemPtr;
