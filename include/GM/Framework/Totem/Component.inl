
template<class ComponentType, class UserData>
Component<ComponentType, UserData>::Component(const std::string &name) 
	: name(name)
{
}

template<class ComponentType, class UserData>
Component<ComponentType, UserData>::~Component()
{
}

template<class ComponentType, class UserData>
unsigned int Component<ComponentType, UserData>::get_runtime_type_id() const
{ 
	return IComponent<UserData>::template get_runtime_type_id<ComponentType>(); 
}

template<class ComponentType, class UserData>
const std::string &Component<ComponentType, UserData>::get_name() const
{ 
	return name; 
}

template<class ComponentType, class UserData>
Component<ComponentType, UserData> &Component<ComponentType, UserData>::operator= (const Component<ComponentType, UserData> &rhs)
{
	if(this == &rhs)
		return *this;

	throw clan::Exception("Assignment operation between ComponentTypes are not supported!");
}
