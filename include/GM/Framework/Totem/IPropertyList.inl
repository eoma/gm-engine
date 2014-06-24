
template<typename PropertyType>
bool IPropertyList::isType(const std::shared_ptr<IPropertyList> &property)
{
	return (property->getRuntimeTypeId() == getRuntimeTypeId<PropertyType>());
}

template<typename PropertyType>
bool IPropertyList::isType(const IPropertyList &property)
{
	return (property.getRuntimeTypeId() == getRuntimeTypeId<PropertyType>());
}

template<typename PropertyType>
unsigned int IPropertyList::getRuntimeTypeId()
{
	static unsigned int typeId(typeid(PropertyType).hash_code());
	return typeId;
}

