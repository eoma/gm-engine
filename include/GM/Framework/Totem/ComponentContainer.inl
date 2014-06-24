
template<class UserData>
ComponentContainer<UserData>::~ComponentContainer()
{
	for(unsigned int i = 0; i < components.size(); i++)
		sign_ComponentRemoved.invoke(components[i]);
}

template<class UserData>
std::shared_ptr<IComponent<UserData>> ComponentContainer<UserData>::add_component(std::shared_ptr<IComponent<UserData>> component)
{
	check_duplication_and_add(component->get_runtime_type_id(), component->get_name());
	components.push_back(component);
	sign_ComponentAdded.invoke(component);
	return component;
}

template<class UserData>
template<class ComponentType>
std::shared_ptr<ComponentType> ComponentContainer<UserData>::add_component(std::shared_ptr<ComponentType> component)
{
	check_duplication_and_add(component->get_runtime_type_id(), component->get_name());
	components.push_back(component);
	sign_ComponentAdded.invoke(component);
	return component;
}

//////////////////////////////////////////////////////////////////////////////

template<class UserData>
template<class ComponentType>
bool ComponentContainer<UserData>::has_component(const std::string &name)
{
	for(unsigned int i = 0; i < components.size(); i++)
	{
		if(IComponent<UserData>::template is_type<ComponentType>(components[i]))
		{
			if(!name.empty())
			{
				if(components[i]->get_name() == name)
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
std::shared_ptr<ComponentType> ComponentContainer<UserData>::get_component(const std::string &name)
{
	for(unsigned int i = 0; i < components.size(); i++)
	{
		if(IComponent<UserData>::template is_type<ComponentType>(components[i]))
		{
			if(!name.empty())
			{
				if(components[i]->get_name() == name)
					return std::static_pointer_cast<ComponentType>(components[i]);
			}
			else
			{
				return std::static_pointer_cast<ComponentType>(components[i]);
			}
		}
	}
	throw clan::Exception(("Couldn't find component with name " + name).c_str());
}

template<class UserData>
std::vector<std::shared_ptr<IComponent<UserData>>> &ComponentContainer<UserData>::get_components() 
{ 
	return components; 
}

template<class UserData>
void ComponentContainer<UserData>::update_components(float elapsed_time)
{
	for(unsigned int i = 0; i < components.size(); i++)
		components[i]->update(elapsed_time);
}

template<class UserData>
template<class ComponentType>
void ComponentContainer<UserData>::remove_component(const std::string &name, bool uphold_order_in_list)
{
	for(unsigned int i = 0; i < components.size(); i++)
	{
		if(IComponent<UserData>::template is_type<ComponentType>(components[i]))
		{
			if(!name.empty())
			{
				if(components[i]->get_name() == name)
				{
					sign_ComponentRemoved.invoke(components[i]);
					if(uphold_order_in_list)
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
				if(uphold_order_in_list)
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
	throw clan::Exception(("Couldn't find component with name " + name).c_str());
}

template<class UserData>
clan::Signal<std::shared_ptr<IComponent<UserData>>> &ComponentContainer<UserData>::component_added() 
{ 
	return sign_ComponentAdded; 
}

template<class UserData>
clan::Signal<std::shared_ptr<IComponent<UserData>>> &ComponentContainer<UserData>::component_removed() 
{ 
	return sign_ComponentRemoved; 
}

template<class UserData>
void ComponentContainer<UserData>::check_duplication_and_add(unsigned int type_id, const std::string &name)
{
	auto it = names_for_component_types.find(type_id);
	if(it != names_for_component_types.end())
	{
		const std::vector<std::string> &names = it->second;
		for(unsigned int i = 0; i < names.size(); i++)
			if(names[i] == name)
				throw clan::Exception("Found duplicate component name: " + name);
		it->second.push_back(name);
	}
	else
	{
		std::vector<std::string> new_list;
		new_list.push_back(name);
		names_for_component_types[type_id] = new_list;
	}
}
