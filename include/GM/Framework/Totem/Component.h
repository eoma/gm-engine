/////////////////////////////////////////////////////
//
#pragma once
#include <ClanLib/core.h>
#pragma warning(push)
#pragma warning(disable : 4481)

#include "IComponent.h"

 
 
 

namespace GM {
namespace Framework {

template<class ComponentType, class UserData = void*>
class Component : public IComponent<UserData> 
{
public:
	Component(const std::string &name);
	virtual ~Component();

	unsigned int get_runtime_type_id() const override;
	const std::string &get_name() const override;

	Component &operator= (const Component &rhs);
	
protected:
	std::string name;
};

#include "Component.inl"

} // namespace Framework
} // namespace GM

#pragma warning(pop)

//
/////////////////////////////////////////////////////////
