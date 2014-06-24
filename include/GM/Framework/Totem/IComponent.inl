
template<typename UserData>
template<typename ComponentType>
bool IComponent<UserData>::is_type(const std::shared_ptr<IComponent> &component)
{
	return (component->get_runtime_type_id() == get_runtime_type_id<ComponentType>());
}

template<typename UserData>
template<typename ComponentType>
bool IComponent<UserData>::is_type(const IComponent &component)
{
	return (component.get_runtime_type_id() == get_runtime_type_id<ComponentType>());
}

template<typename UserData>
template<typename ComponentType>
unsigned int IComponent<UserData>::get_runtime_type_id()
{
	static unsigned int type_id(typeid(ComponentType).hash_code());
	return type_id;
}

