/////////////////////////////////////////////////////
//
#pragma once
#pragma warning(push)
#pragma warning(disable : 4481)

#include "IComponent.h"

#include <sigslot.h>
#include <string>
#include <memory>

namespace Totem
{

template<class ComponentType, class UserData = void*>
class Component : public IComponent<UserData>, public sigslot::has_slots<>
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
