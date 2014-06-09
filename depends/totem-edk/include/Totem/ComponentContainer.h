/////////////////////////////////////////////////////
//
#pragma once
#include <ClanLib/core.h>

#include "IComponent.h"
#include "Component.h"

 
 
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
	template<class ComponentType> bool hasComponent(const CL_String &name = CL_String());
	template<class ComponentType> std::shared_ptr<ComponentType> getComponent(const CL_String &name = CL_String());
	std::vector<std::shared_ptr<IComponent<UserData>>> &getComponents();
	void updateComponents(float elapsedTime);
	template<class ComponentType> void removeComponent(const CL_String &name = CL_String(), bool upholdOrderInList = false);

	CL_Signal_v<std::shared_ptr<IComponent<UserData>>> &componentAdded();
	CL_Signal_v<std::shared_ptr<IComponent<UserData>>> &componentRemoved();

protected:
	void checkDuplicationAndAdd(unsigned int typeId, const CL_String &name);

	std::unordered_map<unsigned int, std::vector<CL_String>> namesForComponentTypes;
	std::vector<std::shared_ptr<IComponent<UserData>>> components;

	CL_Signal_v<std::shared_ptr<IComponent<UserData>>> sign_ComponentAdded;
	CL_Signal_v<std::shared_ptr<IComponent<UserData>>> sign_ComponentRemoved;
};

#include "ComponentContainer.inl"

} //namespace Totem

//
/////////////////////////////////////////////////////////
