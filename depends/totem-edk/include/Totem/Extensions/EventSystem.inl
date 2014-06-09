template<class... Ts> 
inline std::shared_ptr<EventSignal<Ts...>> DefaultEventFactory::createEvent()
{
	return std::make_shared<EventSignal<Ts...>>();
}


template<class EventFactory>
template<class... Ts>
inline void EventSystem<EventFactory>::sendEvent(HashedString type, Ts... args, bool requireReceiver)
{
	auto eventsIt = eventSizeMap.find(sizeof...(args));
	if (eventsIt == eventSizeMap.end()) {
		eventsIt = eventSizeMap.insert(std::make_pair(sizeof...(args), std::make_shared<eventMap>())).first;
	}

	auto events = eventsIt->second;

	auto it = events->find(type.getId());
	if(it == events->end())
	{
		if(requireReceiver)
			throw std::runtime_error(("Couldn't find event type " + type.getStr() + " in events registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal<Ts...>>(it->second);
	if(signal == nullptr)
		throw std::runtime_error(("Tried to invoke event " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(args...);
#else
	std::static_pointer_cast<EventSignal<Ts...>>(it->second)->signal.invoke(args...);
#endif
}

//------------------------------------------------------------------


template<class EventFactory>
template<class... Ts>
inline sigslot::signal<Ts...> &EventSystem<EventFactory>::registerToEvent(HashedString type)
{
	auto eventsIt = eventSizeMap.find(sizeof...(Ts));
	if (eventsIt == eventSizeMap.end()) {
		eventsIt = eventSizeMap.insert(
			std::make_pair(sizeof...(Ts), std::make_shared<eventMap>())).first;
	}

	auto events = eventsIt->second;

	auto it = events->find(type.getId());
	if(it == events->end())
	{
		auto signal = EventFactory::template createEvent<Ts...>();
		events->operator[](type.getId()) = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal<Ts...>>(it->second);
		if(signal == nullptr)
			throw std::runtime_error(("Tried toreturn the event signal " + type.getStr() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal<Ts...>>(it->second);
#endif
		return signal->signal;
	}
}

//---------------------------------------------------------------------------------------

template<class EventFactory>
bool EventSystem<EventFactory>::hasEvent(const HashedString &id, int num_params)
{


	if(num_params >= 0)
	{
		auto eventsIt = eventSizeMap.find(num_params);
		if (eventsIt == eventSizeMap.end()) {
			eventsIt = eventSizeMap.insert(std::make_pair(num_params, std::make_shared<eventMap>())).first;
		}

		auto events = eventsIt->second;

		auto it = events->find(id.getId());

		if (it != events->end()) {
			return true;
		}
		return false;
	}
	else
	{

		for (auto eventsIt = eventSizeMap.begin(); eventsIt != eventSizeMap.end(); ++eventsIt) {
			auto it = eventsIt->second->find(id.getId());
			if (it != eventsIt->second->end()) {
				return true;
			}
		}

		return false;
	}
}
