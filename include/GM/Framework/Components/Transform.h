#pragma once

#include <glm/glm.hpp>

#include <Totem/Component.h>

#include <memory>
#include <string>
#include <tuple>

namespace GM {
namespace Framework {

class Entity; typedef std::shared_ptr<Entity> EntityPtr;

class Transform : public Totem::Component<Transform>
{
public:
	Transform(const EntityPtr &owner, const std::string &name);

protected:

private:
	EntityPtr owner;

	// TODO: Proper defs. of variables
	Totem::Property<glm::vec3> position_property;
	Totem::Property<glm::vec3> scale_property;
	Totem::Property<glm::quat> orientation_property;
};

} // namespace Framework
} // namespace GM
