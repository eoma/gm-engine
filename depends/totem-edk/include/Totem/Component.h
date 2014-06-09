/////////////////////////////////////////////////////
//
#pragma once
#include <ClanLib/core.h>
#pragma warning(push)
#pragma warning(disable : 4481)

#include "IComponent.h"

 
 
 

namespace Totem
{

template<class ComponentType, class UserData = void*>
class Component : public IComponent<UserData> 
{
public:
	Component(const std::string &name);
	virtual ~Component();

	unsigned int getRuntimeTypeId() const override;
	const std::string &getName() const override;

	Component &operator= (const Component &rhs);
	
protected:
	std::string name;
};

#include "Component.inl"

} // namespace Totem

#pragma warning(pop)

//
/////////////////////////////////////////////////////////
