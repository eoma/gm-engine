
template<typename PropertyType>
bool IProperty::is_type(const std::shared_ptr<IProperty> &property)
{
	return (property->get_runtime_type_id() == get_runtime_type_id<PropertyType>());
}
	
template<typename PropertyType>
bool IProperty::is_type(const IProperty &property)
{
	return (property.get_runtime_type_id() == get_runtime_type_id<PropertyType>());
}

template<typename PropertyType>
size_t IProperty::get_runtime_type_id()
{
	static size_t type_id(typeid(PropertyType).hash_code());
	return type_id;
}

