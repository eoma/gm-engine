#include "GM/Framework/Components/Camera.h"

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"

#include "GM/Framework/Systems/RenderSystem.h"

#include <glm/ext.hpp>

using namespace GM::Framework;

Camera::Camera(const EntityPtr &owner, const RenderSystemPtr &render_system, unsigned int render_layers, unsigned int depth, const std::string &name)
: Component(owner, name)
, render_system(render_system)
, render_layers(render_layers)
, depth(depth)
{
	fov_property = owner->add<float>(PROPERTY_FOV, 60.0f);
	near_clipping_property = owner->add<float>(PROPERTY_NEAR_CLIPPING, 0.1f);
	far_clipping_property = owner->add<float>(PROPERTY_FAR_CLIPPING, 1000.0f);

	view_matrix_property = owner->add<glm::mat4>(PROPERTY_VIEW_MATRIX, glm::mat4(1));
	projection_matrix_property = owner->add<glm::mat4>(PROPERTY_PROJECTION_MATRIX, glm::mat4(1));
	world_matrix_property = owner->add<glm::mat4>(PROPERTY_WORLD_MATRIX, glm::mat4(1));
	
	// Calculate default view and projections
	recalculate_view_matrix(world_matrix_property.get(), world_matrix_property.get());
	set_projection(4, 3);

	slots.connect(world_matrix_property.value_changed(), this, &Camera::recalculate_view_matrix);

	render_system->add_camera(this);
}

Camera::~Camera() {
	render_system->remove_camera(this);
}

void Camera::recalculate_view_matrix(const glm::mat4& /* old_world */, const glm::mat4 &new_world) {
	//view_matrix_property = glm::inverse(new_world);
	//view_matrix_property = glm::lookAt(glm::vec3(new_world[3]), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	view_matrix_property = glm::lookAt(glm::vec3(0, 0, 1000), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Camera::clear_dirty() {
	view_matrix_property.clear_dirty();
	projection_matrix_property.clear_dirty();
}

void Camera::set_projection(float fov, float width, float height, float near_clipping, float far_clipping) {
	fov_property = fov;
	near_clipping_property = near_clipping;
	far_clipping_property = far_clipping;

	set_projection(width, height);
}

void Camera::set_projection(const glm::uvec2 &resolution) {
	set_projection((float)resolution.x, (float)resolution.y);
}

void Camera::set_projection(float width, float height) {
	projection_matrix_property = glm::perspectiveFov(fov_property.get(), width, height, near_clipping_property.get(), far_clipping_property.get());
}
