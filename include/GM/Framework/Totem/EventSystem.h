#pragma once
#include <ClanLib/core.h>

#include "HashedString.h"

 
#include <unordered_map>
 

namespace GM {
namespace Framework {
template<class ComponentType, class UserData> class Component;

class IEventSignal { public IEventSignal() {} virtual ~IEventSignal() {} 
};

template<class... Ts>
class EventSignal : public IEventSignal { public: clan::Signal<void(Ts...)> signal; };

class DefaultEventFactory
{
public:
	template<class... Ts> static std::shared_ptr<EventSignal<Ts...>> create_event();
};

template<class EventFactory = DefaultEventFactory>
class EventSystem  
{
public:
	virtual ~EventSystem() {}

	template<class... Ts> void send_event(HashedString type, Ts... args, bool require_receiver = true);

	template<class... Ts> clan::Signal<void(Ts...)> &register_to_event(HashedString type);

	bool has_event(const HashedString &id, int num_params = -1);

protected:

	typedef std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> event_map;

	std::unordered_map<unsigned int, std::shared_ptr<event_map>> event_size_map;
};

#include "EventSystem.inl"

} //namespace GM
} //namespace GM
