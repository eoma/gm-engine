template<class... Ts>
inline std::shared_ptr<EventSignal<Ts...>> DefaultEventFactory::create_event()
{
	return std::make_shared<EventSignal<Ts...>>();
}


template<class EventFactory>
template<class... Ts>
inline void EventSystem<EventFactory>::send_event(HashedString type, Ts... args, bool require_receiver)
{
	auto events_it = event_size_map.find(sizeof...(args));
	if (events_it == event_size_map.end()) {
		events_it = event_size_map.insert(std::make_pair(sizeof...(args), std::make_shared<event_map>())).first;
	}

	auto events = events_it->second;

	auto it = events->find(type.get_id());
	if (it == events->end())
	{
		if (require_receiver)
			throw clan::Exception(("Couldn't find event type " + type.get_str() + " in events registry!").c_str());
		else
			return;
	}

#ifdef _DEBUG
	auto signal = std::dynamic_pointer_cast<EventSignal<Ts...>>(it->second);
	if (signal == nullptr)
		throw clan::Exception(("Tried to invoke event " + type.get_str() + ", but one or both of the argument types didn't match the registered types!").c_str());
	signal->signal.invoke(args...);
#else
	std::static_pointer_cast<EventSignal<Ts...>>(it->second)->signal.invoke(args...);
#endif
}

//------------------------------------------------------------------


template<class EventFactory>
template<class... Ts>
inline clan::Signal<Ts...> &EventSystem<EventFactory>::register_to_event(HashedString type)
{
	auto events_it = event_size_map.find(sizeof...(Ts));
	if (events_it == event_size_map.end()) {
		events_it = event_size_map.insert(
			std::make_pair(sizeof...(Ts), std::make_shared<event_map>())).first;
	}

	auto events = events_it->second;

	auto it = events->find(type.get_id());
	if (it == events->end())
	{
		auto signal = EventFactory::template create_event<Ts...>();
		events->operator[](type.get_id()) = signal;
		return signal->signal;
	}
	else
	{
#ifdef _DEBUG
		auto signal = std::dynamic_pointer_cast<EventSignal<Ts...>>(it->second);
		if (signal == nullptr)
			throw clan::Exception(("Tried toreturn the event signal " + type.get_str() + ", but one or both of the argument types didn't match the registered types!").c_str());
#else
		auto signal = std::static_pointer_cast<EventSignal<Ts...>>(it->second);
#endif
		return signal->signal;
	}
}

//---------------------------------------------------------------------------------------

template<class EventFactory>
bool EventSystem<EventFactory>::has_event(const HashedString &id, int num_params)
{


	if (num_params >= 0)
	{
		auto events_it = event_size_map.find(num_params);
		if (events_it == event_size_map.end()) {
			events_it = event_size_map.insert(std::make_pair(num_params, std::make_shared<event_map>())).first;
		}

		auto events = events_it->second;

		auto it = events->find(id.get_id());

		if (it != events->end()) {
			return true;
		}
		return false;
	}
	else
	{

		for (auto events_it = event_size_map.begin(); events_it != event_size_map.end(); ++events_it) {
			auto it = events_it->second->find(id.get_id());
			if (it != events_it->second->end()) {
				return true;
			}
		}

		return false;
	}
}
