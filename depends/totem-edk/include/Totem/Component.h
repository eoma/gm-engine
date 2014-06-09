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
	Component(const CL_String &name);
	virtual ~Component();

	unsigned int getRuntimeTypeId() const override;
	const CL_String &getName() const override;

	Component &operator= (const Component &rhs);
	
protected:
	CL_String name;
};

#include "Component.inl"

} // namespace Totem

#pragma warning(pop)

//
/////////////////////////////////////////////////////////
