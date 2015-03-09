#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/IRenderPassComponent.h"
#include "GM/Framework/Components/StandardPass.h"
#include "GM/Framework/Components/FinalPass.h"

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"

#include "GM/Framework/Managers/TextureManager.h"

#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Core/GL/FramebufferObject.h"
#include "GM/Core/GL/RenderbufferObject.h"
#include "GM/Core/Utilities/TextureFactory.h"
#include "GM/Core/Utilities/ReadWriteTexture.h"

#include <glm/gtc/matrix_transform.hpp>

#include <GL/gl3w.h>

namespace GM {
namespace Framework {

Camera::Camera(const EntityPtr &owner, const RenderSystemPtr &render_system, const TextureManagerPtr & texture_manager, unsigned int render_layers, int depth, const std::string &name)
: Component(owner, name)
, render_system(render_system)
, render_layers(render_layers)
, depth(depth)
, render_texture(nullptr)
, final_pass(nullptr)
{
	fov_property = owner->add<float>(GM_PROPERTY_FOV, 60.0f);
	near_clipping_property = owner->add<float>(GM_PROPERTY_NEAR_CLIPPING, 0.1f);
	far_clipping_property = owner->add<float>(GM_PROPERTY_FAR_CLIPPING, 1000.0f);
	max_vertical_angle_property = owner->add<float>(GM_PROPERTY_MAX_VERTICAL_ANGLE, 85.0f);

	view_matrix_property = owner->add<glm::mat4>(GM_PROPERTY_VIEW_MATRIX, glm::mat4(1));
	projection_matrix_property = owner->add<glm::mat4>(GM_PROPERTY_PROJECTION_MATRIX, glm::mat4(1));
	world_matrix_property = owner->add<glm::mat4>(GM_PROPERTY_WORLD_MATRIX, glm::mat4(1));

	render_to_screen = owner->add<bool>(GM_PROPERTY_RENDER_TO_SCREEN, true);
	
	// Calculate default view and projections
	recalculate_view_matrix(world_matrix_property.get(), world_matrix_property.get());
	set_projection(4, 3);

	slots.connect(world_matrix_property.value_changed(), this, &Camera::recalculate_view_matrix);

	render_system->add_camera(this);

	render_texture = std::make_shared<Core::ReadWriteTexture>(GL_TEXTURE_2D);
	texture_manager->add(clan::string_format("%1.camera_rt", owner->get_name()), render_texture);

}

Camera::~Camera() {
	render_system->remove_camera(this);
}

void Camera::recalculate_view_matrix(const glm::mat4& /* old_world */, const glm::mat4 &new_world) {
	view_matrix_property = glm::inverse(new_world);
	/*view_matrix_property = glm::lookAt(
		glm::vec3(new_world[3].x, new_world[3].y, new_world[3].z), 
		glm::vec3(new_world[3].x, new_world[3].y, new_world[3].z) - glm::normalize(glm::vec3(new_world[2].x, new_world[2].y, new_world[2].z)),
		glm::normalize(glm::vec3(new_world[1].x, new_world[1].y, new_world[1].z))
		);*/
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
	current_width = width;
	current_height = height;

	projection_matrix_property = glm::perspectiveFov(fov_property.get(), width, height, near_clipping_property.get(), far_clipping_property.get());
}

void Camera::initialize() {
	if (render_texture->get_readable() == nullptr && render_texture->get_writable() == nullptr) {
		Core::TextureFactory::TextureData texture_data;
		Core::TextureFormatPtr texture_format = Core::TextureFormat::create_texture2d_format(false, GL_CLAMP_TO_EDGE);

		texture_data.width = current_width;
		texture_data.height = current_height;
		texture_data.internal_format = GL_RGBA32F;
		texture_data.texture_format = GL_RGBA;

		render_texture->set_readable(Core::TextureFactory::create(*texture_format, texture_data));
		render_texture->set_writable(Core::TextureFactory::create(*texture_format, texture_data));
	}

	// Hopefully all necessary pass components has been added
	make_render_pass_sequence();

	if (pass_sequence.size() == 0) {
		// If not we add the standard pass and initialize ourselvs
		owner->create_component<StandardPass>()->initialize();
		make_render_pass_sequence();
	}

	if (render_to_screen) {
		if (final_pass == nullptr) {
			final_pass = std::make_shared<FinalPass>();
		}

		render_texture->flip();
		final_pass->set_input_texture(render_texture->get_readable());
		final_pass->build();
		pass_sequence.push_back(final_pass.get());
	}
}

void Camera::set_render_layers(unsigned int new_render_layers) {
	render_system->remove_camera(this);

	render_layers = new_render_layers;

	render_system->add_camera(this);
}

void Camera::set_depth(int new_depth) {
	depth = new_depth;
	render_system->sort_cameras();
}

void Camera::make_render_pass_sequence() {
	pass_sequence.clear();

	for (auto &component : owner->get_components()) {
		IRenderPassComponent* pass = dynamic_cast<IRenderPassComponent*>(component.get());

		if (pass != nullptr) {
			pass_sequence.push_back(pass);

			if (pass->uses_render_texture_from_camera()) {
				// Flip textures used as read and write
				render_texture->flip();

				pass->set_input_texture(render_texture->get_readable());
				pass->set_output_texture(render_texture->get_writable());
			}

			pass->build();
		}
	}
}

Core::TexturePtr Camera::get_render_texture() const {
	return render_texture;
}

} // namespace Framework
} // namespace GM
