#pragma once

#include "../HashedString.h"

#include <sigslot.h>
#include <unordered_map>
#include <memory>

namespace Totem {
template<class ComponentType, class UserData> class Component;

class IEventSignal { public: IEventSignal() {} virtual ~IEventSignal() {} };

template<class... Ts>
class EventSignal : public IEventSignal {
public:
	sigslot::signal<Ts...> signal;
};

class DefaultEventFactory
{
public:
	template<class... Ts> static std::shared_ptr<EventSignal<Ts...>> createEvent();
};

template<class EventFactory = DefaultEventFactory>
class EventSystem : public sigslot::has_slots<>
{
public:
	virtual ~EventSystem() {}

	template<class... Ts> void sendEvent(HashedString type, Ts... args, bool requireReceiver = true);

	template<class... Ts> sigslot::signal<Ts...> &registerToEvent(HashedString type);

	bool hasEvent(const HashedString &id, int num_params = -1);

protected:

	typedef std::unordered_map<unsigned int, std::shared_ptr<IEventSignal>> eventMap;

	std::unordered_map<unsigned int, std::shared_ptr<eventMap>> eventSizeMap;
};

#include "EventSystem.inl"

} //namespace Totem
