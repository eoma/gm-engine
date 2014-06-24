#pragma once
#include <ClanLib/core.h>

#include "PropertyContainer.h"

 
 

namespace GM {
namespace Framework {

class Entity;

template<class UserData = void*>
class IComponent : public PropertyContainer<UserData>
{
public:
	IComponent() {}
	virtual ~IComponent() {}
	virtual const std::string &get_name() const = 0;
	virtual const Entity *get_owner() const = 0;
	virtual void update(float /*elapsed_time*/) {}

	template<typename ComponentType> static bool is_type(const std::shared_ptr<IComponent> &component);
	template<typename ComponentType> static bool is_type(const IComponent &component);
	virtual unsigned int get_runtime_type_id() const = 0;
	template<typename ComponentType> static unsigned int get_runtime_type_id();
};

#include "IComponent.inl"

} // namespace Framework
} // namespace GM

