
template<class ComponentType, class UserData>
Component<ComponentType, UserData>::Component(const CL_String &name) 
	: name(name)
{
}

template<class ComponentType, class UserData>
Component<ComponentType, UserData>::~Component()
{
}

template<class ComponentType, class UserData>
unsigned int Component<ComponentType, UserData>::getRuntimeTypeId() const
{ 
	return IComponent<UserData>::template getRuntimeTypeId<ComponentType>(); 
}

template<class ComponentType, class UserData>
const CL_String &Component<ComponentType, UserData>::getName() const
{ 
	return name; 
}

template<class ComponentType, class UserData>
Component<ComponentType, UserData> &Component<ComponentType, UserData>::operator= (const Component<ComponentType, UserData> &rhs)
{
	if(this == &rhs)
		return *this;

	throw CL_Exception("Assignment operation between ComponentTypes are not supported!");
}
