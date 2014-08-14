#include "IdleRotate.h"
#include "GM/Samples/SampleDefinitionsPropertyNames.h"

using namespace GM;
using namespace Samples;

IdleRotationComponent::IdleRotationComponent(const GM::Framework::EntityPtr &owner, const std::string &name)
	: Framework::Component< IdleRotationComponent >(owner, name)
{
	orientation_property = owner->add(GM_PROPERTY_ORIENTATION, glm::quat(
		glm::angleAxis(0.0f, glm::vec3(1, 0, 0)) *
		glm::angleAxis(0.0f, glm::vec3(0, 1, 0)) *
		glm::angleAxis(0.0f, glm::vec3(0, 0, 1))));
}

void IdleRotationComponent::update(float elapsed_time) {
	orientation_property = glm::rotate(orientation_property.get(), elapsed_time, glm::vec3(0, 1, 0));
}
