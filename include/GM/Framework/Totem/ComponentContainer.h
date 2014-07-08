/////////////////////////////////////////////////////
//
#pragma once
#include <ClanLib/core.h>

#include "IComponent.h"
#include "Component.h"

 
 
#include <vector>
#include <unordered_map>

namespace GM {
namespace Framework {

template<class UserData = void*>
class ComponentContainer
{
public:
	ComponentContainer();
	virtual ~ComponentContainer();

	std::shared_ptr<IComponent<UserData>> add_component(std::shared_ptr<IComponent<UserData>> component);
	template<class ComponentType> std::shared_ptr<ComponentType> add_component(std::shared_ptr<ComponentType> component);
	template<class ComponentType> bool has_component(const std::string &name = std::string());
	template<class ComponentType> std::shared_ptr<ComponentType> get_component(const std::string &name = std::string());
	std::vector<std::shared_ptr<IComponent<UserData>>> &get_components();
	void update_components(float elapsed_time);
	template<class ComponentType> void remove_component(const std::string &name = std::string(), bool uphold_order_in_list = false);

	clan::Signal<void(std::shared_ptr<IComponent<UserData>>)> &component_added();
	clan::Signal<void(std::shared_ptr<IComponent<UserData>>)> &component_removed();

protected:
	void check_duplication_and_add(unsigned int type_id, const std::string &name);
	void pre_destruction();

	std::unordered_map<unsigned int, std::vector<std::string>> names_for_component_types;
	std::vector<std::shared_ptr<IComponent<UserData>>> components;

	clan::Signal<void(std::shared_ptr<IComponent<UserData>>)> sign_ComponentAdded;
	clan::Signal<void(std::shared_ptr<IComponent<UserData>>)> sign_ComponentRemoved;
};

#include "ComponentContainer.inl"

} // namespace Framework
} // namespace GM

//
/////////////////////////////////////////////////////////
