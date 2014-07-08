
template<class PropertyType>
std::shared_ptr<GM::Framework::Property<PropertyType>> PropertyFactory::create_property(const std::string &name)
{
	return std::make_shared<GM::Framework::Property<PropertyType>>(name);
}

template<class UserData>
PropertyContainer<UserData>::PropertyContainer() 
: properties()
, deleted_properties()
, sign_PropertyAdded()
, sign_PropertyWithUserDataAdded()
, sign_PropertyRemoved()
, sign_PropertyWithUserDataRemoved()
{
}

template<class UserData>
PropertyContainer<UserData>::~PropertyContainer()
{
	remove_all_properties();
}

template<class UserData>
template<class T>
inline Property<T> PropertyContainer<UserData>::add(const std::string& name, const T &default_value)
{
	auto it = properties.find(name);
	if(it != properties.end())
	{
		std::shared_ptr<Property<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< Property<T> >(it->second);
		if(!property)
			throw clan::Exception(("Property " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyFactory::create_property<T>(name);
	property->set(default_value, true);
	properties[property->get_name()] = property;

	sign_PropertyAdded.invoke(std::static_pointer_cast<IProperty>(property));
	return *property.get();
}

template<class UserData>
template<class T>
inline Property<T> PropertyContainer<UserData>::add(const std::string& name, const T &default_value, const UserData &user_data)
{
	auto it = properties.find(name);
	if(it != properties.end())
	{
		std::shared_ptr<Property<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< Property<T> >(it->second);
		if(!property)
			throw clan::Exception(("Property " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyFactory::create_property<T>(name);
	property->set(default_value, true);
	properties[property->get_name()] = property;

	sign_PropertyWithUserDataAdded.invoke(std::static_pointer_cast<IProperty>(property), user_data);
	return *property.get();
}

template<class UserData>
template<class T>
inline Property<T> PropertyContainer<UserData>::get(const std::string& name)
{
	auto it = properties.find(name);
	if(it != properties.end())
	{
		std::shared_ptr<Property<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< Property<T> >(it->second);
		if(!property)
			throw clan::Exception(("Tried to get shared property " + name + ", but the type was wrong!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}
	else
		throw clan::Exception(("Unable to get property " + name).c_str());
}

template<class UserData>
inline std::unordered_map<std::string, std::shared_ptr<IProperty>> &PropertyContainer<UserData>::get_properties() 
{ 
	return properties; 
}

template<class UserData>
inline PropertyContainer<UserData> &PropertyContainer<UserData>::operator= (const PropertyContainer<UserData> &rhs)
{
	if(this == &rhs)
		return *this;

	throw clan::Exception("Assignment operation between PropertyContainer are not supported!");
}

template<class UserData>
inline clan::Signal<std::shared_ptr<IProperty>> &PropertyContainer<UserData>::property_added() 
{ 
	return sign_PropertyAdded; 
}

template<class UserData>
inline clan::Signal<std::shared_ptr<IProperty>, const UserData&> &PropertyContainer<UserData>::property_with_user_data_added() 
{ 
	return sign_PropertyWithUserDataAdded; 
}

template<class UserData>
inline clan::Signal<std::shared_ptr<IProperty>> &PropertyContainer<UserData>::property_removed() 
{ 
	return sign_PropertyRemoved; 
}

template<class UserData>
inline clan::Signal<std::shared_ptr<IProperty>, const UserData&> &PropertyContainer<UserData>::property_with_user_data_removed()
{
	return sign_PropertyWithUserDataRemoved;
}

template<class UserData>
inline bool PropertyContainer<UserData>::has_property(const std::string& name)
{
if(properties.empty())
	return false;

auto it = properties.find(name);
if(it != properties.end())
	return true;
else
	return false;
}

template<class UserData>
inline void PropertyContainer<UserData>::add(std::shared_ptr<IProperty> property)
{
auto it = properties.find(property->get_name());
if(it == properties.end())
	properties[property->get_name()] = property;
}

template<class UserData>
inline void PropertyContainer<UserData>::remove_property(const std::string& name, bool postpone_delete)
{
auto it = properties.find(name);
if(it != properties.end())
{
	std::shared_ptr<IProperty> property = (*it).second;
	if(postpone_delete)
		deleted_properties.push_back(property);
	properties.erase(it);

	sign_PropertyRemoved.invoke(property);
}
}

template<class UserData>
inline void PropertyContainer<UserData>::remove_property(const std::string& name, const UserData &user_data, bool postpone_delete)
{
auto it = properties.find(name);
if(it != properties.end())
{
	std::shared_ptr<IProperty> property = (*it).second;
	if(postpone_delete)
		deleted_properties.push_back(property);
	properties.erase(it);

	sign_PropertyWithUserDataRemoved.invoke(property, user_data);
}
}

template<class UserData>
inline void PropertyContainer<UserData>::remove_all_properties()
{
for(auto it = properties.begin(); it != properties.end(); ++it)
	sign_PropertyRemoved.invoke(it->second);

	properties.clear();
	clear_deleted_properties();
}

template<class UserData>
inline void PropertyContainer<UserData>::remove_all_properties(const UserData &user_data)
{
for(auto it = properties.begin(); it != properties.end(); ++it)
	sign_PropertyWithUserDataRemoved.invoke(it->second, user_data);

	properties.clear();
	clear_deleted_properties();
}

template<class UserData>
inline void PropertyContainer<UserData>::update_properties()
{
	clear_deleted_properties();
}

template<class UserData>
inline void PropertyContainer<UserData>::clear_deleted_properties()
{
	deleted_properties.clear();
}

template<class UserData>
inline void PropertyContainer<UserData>::clear_dirty_properties()
{
for(auto it = properties.begin(); it != properties.end(); ++it)
	it->second->clear_dirty();
}
