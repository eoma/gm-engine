
template<class UserData>
ComponentContainer<UserData>::~ComponentContainer()
{
	for(unsigned int i = 0; i < components.size(); i++)
		sign_ComponentRemoved.invoke(components[i]);
}

template<class UserData>
std::shared_ptr<IComponent<UserData>> ComponentContainer<UserData>::addComponent(std::shared_ptr<IComponent<UserData>> component)
{
	checkDuplicationAndAdd(component->getRuntimeTypeId(), component->getName());
	components.push_back(component);
	sign_ComponentAdded.invoke(component);
	return component;
}

template<class UserData>
template<class ComponentType>
std::shared_ptr<ComponentType> ComponentContainer<UserData>::addComponent(std::shared_ptr<ComponentType> component)
{
	checkDuplicationAndAdd(component->getRuntimeTypeId(), component->getName());
	components.push_back(component);
	sign_ComponentAdded.invoke(component);
	return component;
}

//////////////////////////////////////////////////////////////////////////////

template<class UserData>
template<class ComponentType>
bool ComponentContainer<UserData>::hasComponent(const std::string &name)
{
	for(unsigned int i = 0; i < components.size(); i++)
	{
		if(IComponent<UserData>::template isType<ComponentType>(components[i]))
		{
			if(!name.empty())
			{
				if(components[i]->getName() == name)
					return true;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

template<class UserData>
template<class ComponentType>
std::shared_ptr<ComponentType> ComponentContainer<UserData>::getComponent(const std::string &name)
{
	for(unsigned int i = 0; i < components.size(); i++)
	{
		if(IComponent<UserData>::template isType<ComponentType>(components[i]))
		{
			if(!name.empty())
			{
				if(components[i]->getName() == name)
					return std::static_pointer_cast<ComponentType>(components[i]);
			}
			else
			{
				return std::static_pointer_cast<ComponentType>(components[i]);
			}
		}
	}
	throw std::runtime_error(("Couldn't find component with name " + name).c_str());
}

template<class UserData>
std::vector<std::shared_ptr<IComponent<UserData>>> &ComponentContainer<UserData>::getComponents() 
{ 
	return components; 
}

template<class UserData>
void ComponentContainer<UserData>::updateComponents(float elapsedTime)
{
	for(unsigned int i = 0; i < components.size(); i++)
		components[i]->update(elapsedTime);
}

template<class UserData>
template<class ComponentType>
void ComponentContainer<UserData>::removeComponent(const std::string &name, bool upholdOrderInList)
{
	for(unsigned int i = 0; i < components.size(); i++)
	{
		if(IComponent<UserData>::template isType<ComponentType>(components[i]))
		{
			if(!name.empty())
			{
				if(components[i]->getName() == name)
				{
					sign_ComponentRemoved.invoke(components[i]);
					if(upholdOrderInList)
					{
						components.erase(components.begin()+i);
					}
					else
					{
						components[i] = components.back();
						components.pop_back();
					}
					return;
				}
			}
			else
			{
				sign_ComponentRemoved.invoke(components[i]);
				if(upholdOrderInList)
				{
					components.erase(components.begin()+i);
				}
				else
				{
					components[i] = components.back();
					components.pop_back();
				}
				return;
			}
		}
	}
	throw std::runtime_error(("Couldn't find component with name " + name).c_str());
}

template<class UserData>
sigslot::signal<std::shared_ptr<IComponent<UserData>>> &ComponentContainer<UserData>::componentAdded() 
{ 
	return sign_ComponentAdded; 
}

template<class UserData>
sigslot::signal<std::shared_ptr<IComponent<UserData>>> &ComponentContainer<UserData>::componentRemoved() 
{ 
	return sign_ComponentRemoved; 
}

template<class UserData>
void ComponentContainer<UserData>::checkDuplicationAndAdd(unsigned int typeId, const std::string &name)
{
	auto it = namesForComponentTypes.find(typeId);
	if(it != namesForComponentTypes.end())
	{
		const std::vector<std::string> &names = it->second;
		for(unsigned int i = 0; i < names.size(); i++)
			if(names[i] == name)
				throw std::runtime_error("Found duplicate component name: " + name);
		it->second.push_back(name);
	}
	else
	{
		std::vector<std::string> new_list;
		new_list.push_back(name);
		namesForComponentTypes[typeId] = new_list;
	}
}
