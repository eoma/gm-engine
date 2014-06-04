#pragma once

class TurnListener
{
public:
    TurnListener(const std::string &group);

    std::string get_group() const;

    virtual void on_turn() = 0;

private:
    std::string group;
};
typedef std::shared_ptr<TurnListener> TurnListenerPtr;
