/////////////////////////////////////////////////////
//
#pragma once

#include "IComponent.h"
#include "Component.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace Totem
{

template<class UserData = void*>
class ComponentContainer
{
public:
	virtual ~ComponentContainer();

	std::shared_ptr<IComponent<UserData>> addComponent(std::shared_ptr<IComponent<UserData>> component);
	template<class ComponentType> std::shared_ptr<ComponentType> addComponent(std::shared_ptr<ComponentType> component);
	template<class ComponentType> bool hasComponent(const std::string &name = std::string());
	template<class ComponentType> std::shared_ptr<ComponentType> getComponent(const std::string &name = std::string());
	std::vector<std::shared_ptr<IComponent<UserData>>> &getComponents();
	void updateComponents(float elapsedTime);
	template<class ComponentType> void removeComponent(const std::string &name = std::string(), bool upholdOrderInList = false);

	sigslot::signal<std::shared_ptr<IComponent<UserData>>> &componentAdded();
	sigslot::signal<std::shared_ptr<IComponent<UserData>>> &componentRemoved();

protected:
	void checkDuplicationAndAdd(unsigned int typeId, const std::string &name);

	std::unordered_map<unsigned int, std::vector<std::string>> namesForComponentTypes;
	std::vector<std::shared_ptr<IComponent<UserData>>> components;

	sigslot::signal<std::shared_ptr<IComponent<UserData>>> sign_ComponentAdded;
	sigslot::signal<std::shared_ptr<IComponent<UserData>>> sign_ComponentRemoved;
};

#include "ComponentContainer.inl"

} //namespace Totem

//
/////////////////////////////////////////////////////////
