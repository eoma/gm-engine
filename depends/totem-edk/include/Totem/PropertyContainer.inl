
template<class PropertyType>
std::shared_ptr<Totem::Property<PropertyType>> PropertyFactory::createProperty(const std::string &name)
{
	return std::make_shared<Totem::Property<PropertyType>>(name);
}

template<class UserData>
PropertyContainer<UserData>::PropertyContainer() 
{
}

template<class UserData>
PropertyContainer<UserData>::~PropertyContainer()
{
	removeAllProperties();
}

template<class UserData>
template<class T>
inline Property<T> PropertyContainer<UserData>::add(const std::string& name, const T &defaultValue)
{
	auto it = properties.find(name);
	if(it != properties.end())
	{
		std::shared_ptr<Property<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< Property<T> >(it->second);
		if(!property)
			throw std::runtime_error(("Property " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyFactory::createProperty<T>(name);
	property->set(defaultValue, true);
	properties[property->getName()] = property;

	sign_PropertyAdded.invoke(std::static_pointer_cast<IProperty>(property));
	return *property.get();
}

template<class UserData>
template<class T>
inline Property<T> PropertyContainer<UserData>::add(const std::string& name, const T &defaultValue, const UserData &userData)
{
	auto it = properties.find(name);
	if(it != properties.end())
	{
		std::shared_ptr<Property<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< Property<T> >(it->second);
		if(!property)
			throw std::runtime_error(("Property " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyFactory::createProperty<T>(name);
	property->set(defaultValue, true);
	properties[property->getName()] = property;

	sign_PropertyWithUserDataAdded.invoke(std::static_pointer_cast<IProperty>(property), userData);
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
			throw std::runtime_error(("Tried to get shared property " + name + ", but the type was wrong!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}
	else
		throw std::runtime_error(("Unable to get property " + name).c_str());
}

template<class UserData>
inline std::unordered_map<std::string, std::shared_ptr<IProperty>> &PropertyContainer<UserData>::getProperties() 
{ 
	return properties; 
}

template<class UserData>
inline PropertyContainer<UserData> &PropertyContainer<UserData>::operator= (const PropertyContainer<UserData> &rhs)
{
	if(this == &rhs)
		return *this;

	throw std::runtime_error("Assignment operation between PropertyContainer are not supported!");
}

template<class UserData>
inline sigslot::signal<std::shared_ptr<IProperty>> &PropertyContainer<UserData>::propertyAdded() 
{ 
	return sign_PropertyAdded; 
}

template<class UserData>
inline sigslot::signal<std::shared_ptr<IProperty>, const UserData&> &PropertyContainer<UserData>::propertyWithUserDataAdded() 
{ 
	return sign_PropertyWithUserDataAdded; 
}

template<class UserData>
inline sigslot::signal<std::shared_ptr<IProperty>> &PropertyContainer<UserData>::propertyRemoved() 
{ 
	return sign_PropertyRemoved; 
}

template<class UserData>
inline sigslot::signal<std::shared_ptr<IProperty>, const UserData&> &PropertyContainer<UserData>::propertyWithUserDataRemoved()
{
	return sign_PropertyWithUserDataRemoved;
}

template<class UserData>
inline bool PropertyContainer<UserData>::hasProperty(const std::string& name)
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
auto it = properties.find(property->getName());
if(it == properties.end())
	properties[property->getName()] = property;
}

template<class UserData>
inline void PropertyContainer<UserData>::removeProperty(const std::string& name, bool postponeDelete)
{
auto it = properties.find(name);
if(it != properties.end())
{
	std::shared_ptr<IProperty> property = (*it).second;
	if(postponeDelete)
		deletedProperties.push_back(property);
	properties.erase(it);

	sign_PropertyRemoved.invoke(property);
}
}

template<class UserData>
inline void PropertyContainer<UserData>::removeProperty(const std::string& name, const UserData &userData, bool postponeDelete)
{
auto it = properties.find(name);
if(it != properties.end())
{
	std::shared_ptr<IProperty> property = (*it).second;
	if(postponeDelete)
		deletedProperties.push_back(property);
	properties.erase(it);

	sign_PropertyWithUserDataRemoved.invoke(property, userData);
}
}

template<class UserData>
inline void PropertyContainer<UserData>::removeAllProperties()
{
for(auto it = properties.begin(); it != properties.end(); ++it)
	sign_PropertyRemoved.invoke(it->second);

properties.clear();
clearDeletedProperties();
}

template<class UserData>
inline void PropertyContainer<UserData>::removeAllProperties(const UserData &userData)
{
for(auto it = properties.begin(); it != properties.end(); ++it)
	sign_PropertyWithUserDataRemoved.invoke(it->second, userData);

properties.clear();
clearDeletedProperties();
}

template<class UserData>
inline void PropertyContainer<UserData>::updateProperties()
{
clearDeletedProperties();
}

template<class UserData>
inline void PropertyContainer<UserData>::clearDeletedProperties()
{
deletedProperties.clear();
}

template<class UserData>
inline void PropertyContainer<UserData>::clearDirtyProperties()
{
for(auto it = properties.begin(); it != properties.end(); ++it)
	it->second->clearDirty();
}
