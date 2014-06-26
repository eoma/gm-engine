#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"

#include <glm/ext.hpp>

using namespace GM::Framework;

Camera::Camera(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name)
: Component(owner, name)
, render_system(render_system)
{

	view_matrix_property = owner->add<glm::mat4>(PROPERTY_VIEW_MATRIX, glm::mat4());
	projection_matrix_property = owner->add<glm::mat4>(PROPERTY_PROJECTION_MATRIX, glm::mat4());
	world_matrix_property = owner->add<glm::mat4>(PROPERTY_WORLD_MATRIX, glm::mat4());

	slots.connect(world_matrix_property.value_changed(), clan::Callback<void(const glm::mat4&, const glm::mat4&)>(this, &Camera::recalculate_view_matrix));
}

Camera::~Camera() {

}

void Camera::recalculate_view_matrix(const glm::mat4& /* old_world */, const glm::mat4 &new_world) {
	view_matrix_property = glm::inverse(new_world);
}

void Camera::clear_dirty() {
	view_matrix_property.clear_dirty();
	projection_matrix_property.clear_dirty();
}
