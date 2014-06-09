
template<class PropertyType>
std::shared_ptr<Totem::PropertyList<PropertyType>> PropertyListFactory::createPropertyList(const CL_String &name)
{
	return std::make_shared<Totem::PropertyList<PropertyType>>(name);
}

template<class UserData>
PropertyListContainer<UserData>::PropertyListContainer() 
{
}

template<class UserData>
PropertyListContainer<UserData>::~PropertyListContainer()
{
	removeAllPropertyLists();
}

template<class UserData>
template<class T>
PropertyList<T> PropertyListContainer<UserData>::addList(const CL_String& name)
{
	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
	{
		std::shared_ptr<PropertyList<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< PropertyList<T> >(it->second);
		if(!property)
			throw CL_Exception(("PropertyList " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< PropertyList<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyListFactory::createPropertyList<T>(name);
	propertyLists[property->getName()] = property;

	//return *property;
	sign_PropertyListAdded.invoke(std::static_pointer_cast<IPropertyList>(property));
	//return getProperty<T>(name);
	return *property.get();
}

template<class UserData>
template<class T>
PropertyList<T> PropertyListContainer<UserData>::addList(const CL_String& name, const UserData &userData)
{
	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
	{
		std::shared_ptr<PropertyList<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< PropertyList<T> >(it->second);
		if(!property)
			throw CL_Exception(("PropertyList " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyListFactory::createPropertyList<T>(name);
	propertyLists[property->getName()] = property;

	//return *property;
	sign_PropertyListWithUserDataAdded.invoke(std::static_pointer_cast<IPropertyList>(property), userData);
	//return getProperty<T>(name);
	return *property.get();
}

template<class UserData>
template<class T>
PropertyList<T> PropertyListContainer<UserData>::getList(const CL_String& name)
{
	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
	{
		std::shared_ptr<PropertyList<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< PropertyList<T> >(it->second);
		if(!property)
			throw CL_Exception(("Tried to get shared property list " + name + ", but the type was wrong!").c_str());
#else
		property = std::static_pointer_cast< PropertyList<T> >(it->second);
#endif
		return *property.get();
	}
	else
		throw CL_Exception(("Unable to get shared property list " + name).c_str());
}

template<class UserData>
std::unordered_map<CL_String, std::shared_ptr<IPropertyList>> &PropertyListContainer<UserData>::getPropertyLists()
{
	return propertyLists; 
}

template<class UserData>
PropertyListContainer<UserData> &PropertyListContainer<UserData>::operator= (const PropertyListContainer<UserData> &rhs)
{
	if(this == &rhs)
		return *this;

	throw CL_Exception("Assignment operation between PropertyListContainer are not supported!");
}

template<class UserData>
CL_Signal_v<std::shared_ptr<IPropertyList>> &PropertyListContainer<UserData>::propertyListAdded() 
{
	return sign_PropertyListAdded;
}

template<class UserData>
CL_Signal_v<std::shared_ptr<IPropertyList>, const UserData&> &PropertyListContainer<UserData>::propertyListWithUserDataAdded() 
{
	return sign_PropertyListWithUserDataAdded; 
}

template<class UserData>
CL_Signal_v<std::shared_ptr<IPropertyList>> &PropertyListContainer<UserData>::propertyListRemoved()
{
	return sign_PropertyListRemoved;
}

template<class UserData>
CL_Signal_v<std::shared_ptr<IPropertyList>, const UserData&> &PropertyListContainer<UserData>::propertyListWithUserDataRemoved()
{
	return sign_PropertyListWithUserDataRemoved;
}

template<class UserData>
inline bool PropertyListContainer<UserData>::hasPropertyList(const CL_String& name)
{
	if(propertyLists.empty())
		return false;

	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
		return true;
	else
		return false;
}

template<class UserData>
inline void PropertyListContainer<UserData>::addList(std::shared_ptr<IPropertyList> property)
{
	auto it = propertyLists.find(property->getName());
	if(it == propertyLists.end())
		propertyLists[property->getName()] = property;
}

template<class UserData>
inline std::shared_ptr<IPropertyList> PropertyListContainer<UserData>::getListInterface(const CL_String& name)
{
	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
		return it->second;
	else
		throw CL_Exception(("Unable to get shared property list " + name).c_str());
}

template<class UserData>
inline void PropertyListContainer<UserData>::removePropertyList(const CL_String& name, bool postponeDelete)
{
	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
	{
		std::shared_ptr<IPropertyList> property = (*it).second;
		if(postponeDelete)
			deletedPropertyLists.push_back(property);
		propertyLists.erase(it);

		sign_PropertyListRemoved.invoke(property);
	}
}

template<class UserData>
inline void PropertyListContainer<UserData>::removePropertyList(const CL_String& name, const UserData &userData, bool postponeDelete)
{
	auto it = propertyLists.find(name);
	if(it != propertyLists.end())
	{
		std::shared_ptr<IPropertyList> property = (*it).second;
		if(postponeDelete)
			deletedPropertyLists.push_back(property);
		propertyLists.erase(it);

		sign_PropertyListWithUserDataRemoved.invoke(property, userData);
	}
}

template<class UserData>
inline void PropertyListContainer<UserData>::removeAllPropertyLists()
{
	propertyLists.clear();
	clearDeletedPropertyLists();
}

template<class UserData>
inline void PropertyListContainer<UserData>::updatePropertyLists()
{
	clearDeletedPropertyLists();
}

template<class UserData>
inline void PropertyListContainer<UserData>::clearDeletedPropertyLists()
{
	deletedPropertyLists.clear();
}

template<class UserData>
inline void PropertyListContainer<UserData>::clearDirtyPropertyLists()
{
	for(auto it = propertyLists.begin(); it != propertyLists.end(); ++it)
		it->second->clearDirty();
}
