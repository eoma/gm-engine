
template<class PropertyType>
std::shared_ptr<GM::Framework::PropertyList<PropertyType>> PropertyListFactory::create_property_list(const std::string &name)
{
	return std::make_shared<GM::Framework::PropertyList<PropertyType>>(name);
}

template<class UserData>
PropertyListContainer<UserData>::PropertyListContainer() 
{
}

template<class UserData>
PropertyListContainer<UserData>::~PropertyListContainer()
{
	remove_all_property_lists();
}

template<class UserData>
template<class T>
PropertyList<T> PropertyListContainer<UserData>::add_list(const std::string& name)
{
	auto it = property_lists.find(name);
	if(it != property_lists.end())
	{
		std::shared_ptr<PropertyList<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< PropertyList<T> >(it->second);
		if(!property)
			throw clan::Exception(("PropertyList " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< PropertyList<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyListFactory::create_property_list<T>(name);
	property_lists[property->get_name()] = property;

	//return *property;
	sign_PropertyListAdded.invoke(std::static_pointer_cast<IPropertyList>(property));
	//return get_property<T>(name);
	return *property.get();
}

template<class UserData>
template<class T>
PropertyList<T> PropertyListContainer<UserData>::add_list(const std::string& name, const UserData &user_data)
{
	auto it = property_lists.find(name);
	if(it != property_lists.end())
	{
		std::shared_ptr<PropertyList<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< PropertyList<T> >(it->second);
		if(!property)
			throw clan::Exception(("PropertyList " + name + " already exists, but with another type!").c_str());
#else
		property = std::static_pointer_cast< Property<T> >(it->second);
#endif
		return *property.get();
	}

	auto property = PropertyListFactory::create_property_list<T>(name);
	property_lists[property->get_name()] = property;

	//return *property;
	sign_PropertyListWithUserDataAdded.invoke(std::static_pointer_cast<IPropertyList>(property), user_data);
	//return get_property<T>(name);
	return *property.get();
}

template<class UserData>
template<class T>
PropertyList<T> PropertyListContainer<UserData>::get_list(const std::string& name)
{
	auto it = property_lists.find(name);
	if(it != property_lists.end())
	{
		std::shared_ptr<PropertyList<T>> property;
#ifdef _DEBUG
		property = std::dynamic_pointer_cast< PropertyList<T> >(it->second);
		if(!property)
			throw clan::Exception(("Tried to get shared property list " + name + ", but the type was wrong!").c_str());
#else
		property = std::static_pointer_cast< PropertyList<T> >(it->second);
#endif
		return *property.get();
	}
	else
		throw clan::Exception(("Unable to get shared property list " + name).c_str());
}

template<class UserData>
std::unordered_map<std::string, std::shared_ptr<IPropertyList>> &PropertyListContainer<UserData>::get_property_lists()
{
	return property_lists; 
}

template<class UserData>
PropertyListContainer<UserData> &PropertyListContainer<UserData>::operator= (const PropertyListContainer<UserData> &rhs)
{
	if(this == &rhs)
		return *this;

	throw clan::Exception("Assignment operation between PropertyListContainer are not supported!");
}

template<class UserData>
clan::Signal<std::shared_ptr<IPropertyList>> &PropertyListContainer<UserData>::property_list_added() 
{
	return sign_PropertyListAdded;
}

template<class UserData>
clan::Signal<std::shared_ptr<IPropertyList>, const UserData&> &PropertyListContainer<UserData>::property_list_with_user_data_added() 
{
	return sign_PropertyListWithUserDataAdded; 
}

template<class UserData>
clan::Signal<std::shared_ptr<IPropertyList>> &PropertyListContainer<UserData>::property_list_removed()
{
	return sign_PropertyListRemoved;
}

template<class UserData>
clan::Signal<std::shared_ptr<IPropertyList>, const UserData&> &PropertyListContainer<UserData>::property_list_with_user_data_removed()
{
	return sign_PropertyListWithUserDataRemoved;
}

template<class UserData>
inline bool PropertyListContainer<UserData>::has_property_list(const std::string& name)
{
	if(property_lists.empty())
		return false;

	auto it = property_lists.find(name);
	if(it != property_lists.end())
		return true;
	else
		return false;
}

template<class UserData>
inline void PropertyListContainer<UserData>::add_list(std::shared_ptr<IPropertyList> property)
{
	auto it = property_lists.find(property->get_name());
	if(it == property_lists.end())
		property_lists[property->get_name()] = property;
}

template<class UserData>
inline std::shared_ptr<IPropertyList> PropertyListContainer<UserData>::get_list_interface(const std::string& name)
{
	auto it = property_lists.find(name);
	if(it != property_lists.end())
		return it->second;
	else
		throw clan::Exception(("Unable to get shared property list " + name).c_str());
}

template<class UserData>
inline void PropertyListContainer<UserData>::remove_property_list(const std::string& name, bool postpone_delete)
{
	auto it = property_lists.find(name);
	if(it != property_lists.end())
	{
		std::shared_ptr<IPropertyList> property = (*it).second;
		if(postpone_delete)
			deleted_property_lists.push_back(property);
		property_lists.erase(it);

		sign_PropertyListRemoved.invoke(property);
	}
}

template<class UserData>
inline void PropertyListContainer<UserData>::remove_property_list(const std::string& name, const UserData &user_data, bool postpone_delete)
{
	auto it = property_lists.find(name);
	if(it != property_lists.end())
	{
		std::shared_ptr<IPropertyList> property = (*it).second;
		if(postpone_delete)
			deleted_property_lists.push_back(property);
		property_lists.erase(it);

		sign_PropertyListWithUserDataRemoved.invoke(property, user_data);
	}
}

template<class UserData>
inline void PropertyListContainer<UserData>::remove_all_property_lists()
{
	property_lists.clear();
	clear_deleted_property_lists();
}

template<class UserData>
inline void PropertyListContainer<UserData>::update_property_lists()
{
	clear_deleted_property_lists();
}

template<class UserData>
inline void PropertyListContainer<UserData>::clear_deleted_property_lists()
{
	deleted_property_lists.clear();
}

template<class UserData>
inline void PropertyListContainer<UserData>::clear_dirty_property_lists()
{
	for(auto it = property_lists.begin(); it != property_lists.end(); ++it)
		it->second->clear_dirty();
}
