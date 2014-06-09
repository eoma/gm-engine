
template<typename UserData>
template<typename ComponentType>
bool IComponent<UserData>::isType(const std::shared_ptr<IComponent> &component)
{
	return (component->getRuntimeTypeId() == getRuntimeTypeId<ComponentType>());
}

template<typename UserData>
template<typename ComponentType>
bool IComponent<UserData>::isType(const IComponent &component)
{
	return (component.getRuntimeTypeId() == getRuntimeTypeId<ComponentType>());
}

template<typename UserData>
template<typename ComponentType>
unsigned int IComponent<UserData>::getRuntimeTypeId()
{
	static unsigned int typeId(typeid(ComponentType).hash_code());
	return typeId;
}

