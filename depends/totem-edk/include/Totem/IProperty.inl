
template<typename PropertyType>
bool IProperty::isType(const std::shared_ptr<IProperty> &property)
{
	return (property->getRuntimeTypeId() == getRuntimeTypeId<PropertyType>());
}
	
template<typename PropertyType>
bool IProperty::isType(const IProperty &property)
{
	return (property.getRuntimeTypeId() == getRuntimeTypeId<PropertyType>());
}

template<typename PropertyType>
unsigned int IProperty::getRuntimeTypeId()
{
	static unsigned int typeId(typeid(PropertyType).hash_code());
	return typeId;
}

