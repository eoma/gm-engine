/////////////////////////////////////////////////////
//
#pragma once
#include <ClanLib/core.h>

#if defined(_MSC_VER) // for visual studio
#    pragma warning(push)
#    pragma warning(disable : 4481)
#endif

#include "IComponent.h"

 
 
 

namespace GM {
namespace Framework {

class Entity; typedef std::shared_ptr<Entity> EntityPtr;

template<class ComponentType, class UserData = void*>
class Component : public IComponent<UserData> 
{
public:
	Component(const EntityPtr &owner, const std::string &name);
	virtual ~Component();

	unsigned int get_runtime_type_id() const override;
	const std::string &get_name() const override;
	virtual const Entity *get_owner() const override;
	virtual Entity *get_owner() override;

	Component &operator= (const Component &rhs);
	
protected:
	Entity *owner;
	std::string name;
};

#include "Component.inl"

} // namespace Framework
} // namespace GM

#if defined(_MSC_VER)
#    pragma warning(pop)
#endif

//
/////////////////////////////////////////////////////////
