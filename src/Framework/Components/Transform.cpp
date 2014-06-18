#include "GM/Framework/Components/Transform.h"

#include <glm/ext.hpp>

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"

using namespace GM::Framework;
using namespace Totem;

Transform::Transform(const EntityPtr &owner, const std::string &name)
: Component(name)
, owner(owner)
{
	position_property = owner->add(TRANSFORM_POSITION, glm::vec3());
	scale_property = owner->add(TRANSFORM_SCALE, glm::vec3());
	orientation_property = owner->add(TRANSFORM_ORIENTATION, glm::quat());
}
