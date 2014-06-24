
template<typename PropertyType>
bool IPropertyList::is_type(const std::shared_ptr<IPropertyList> &property)
{
	return (property->get_runtime_type_id() == get_runtime_type_id<PropertyType>());
}

template<typename PropertyType>
bool IPropertyList::is_type(const IPropertyList &property)
{
	return (property.get_runtime_type_id() == get_runtime_type_id<PropertyType>());
}

template<typename PropertyType>
unsigned int IPropertyList::get_runtime_type_id()
{
	static unsigned int type_id(typeid(PropertyType).hash_code());
	return type_id;
}

