#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Managers/MaterialManager.h"
#include "GM/Framework/Managers/MeshManager.h"
#include "GM/Framework/Systems/RenderSystem.h"
#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Utilities/PropertyToUniformConnector.h"

#include "GM/Core/GL/Render.h"
#include "GM/Core/GL/Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace GM {
namespace Framework {

Renderable::Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const MaterialManagerPtr material_manager, const MeshManagerPtr mesh_manager, const unsigned int render_layers, const std::string &name)
: Component<Renderable>(owner, name)
, render_system(render_system)
, material_manager(material_manager)
, mesh_manager(mesh_manager)
, render_layers(render_layers)
{
	render_system->add_renderable(this);

	world_matrix_property = owner->add(GM_PROPERTY_WORLD_MATRIX, glm::mat4(1));
	object_matrix_property = owner->add(GM_PROPERTY_OBJECT_MATRIX, glm::mat4(1));
	normal_matrix_property = owner->add(GM_PROPERTY_NORMAL_MATRIX, glm::mat3(1));

	culled_property = owner->add(GM_PROPERTY_CULLED, false);
	visible_property = owner->add(GM_PROPERTY_VISIBLE, false);

	material_name_property = owner->add(GM_PROPERTY_MATERIAL_NAME, std::string());
	slots.connect(material_name_property.value_changed(),
		[this](const std::string &/*old_material_name*/, const std::string &new_material_name)
		{
			if (new_material_name.empty())
			{
				uniforms_for_render_pass.clear();
				this->material = nullptr;
			}
			else
			{
				this->material = this->material_manager->get_or_create(new_material_name);
				this->set_up_uniform_listeners();

				if (this->material->has_property(GM_PROPERTY_MESH_PIVOT_POINT_OFFSET)) {
					this->material->get<glm::vec3>(GM_PROPERTY_MESH_PIVOT_POINT_OFFSET) = mesh_pivot_point_offset_property.get();
				}
			}
		});

	mesh_name_property = owner->add(GM_PROPERTY_MESH_NAME, std::string());
	slots.connect(mesh_name_property.value_changed(),
		[this](const std::string &/*old_mesh_name*/, const std::string &new_mesh_name) mutable
		{
			if (new_mesh_name.empty())
			{
				mesh = nullptr;
			}
			else
			{
				mesh = this->mesh_manager->get_or_create(new_mesh_name);
			}
		});

	mesh_pivot_point_offset_property = owner->add(GM_PROPERTY_MESH_PIVOT_POINT_OFFSET, glm::vec3());
	slots.connect(mesh_pivot_point_offset_property.value_changed(), 
		[this](const glm::vec3 &/*old_value*/, const glm::vec3 &new_value) mutable 
		{
			if (this->material != nullptr) {
				if (this->material->has_property(GM_PROPERTY_MESH_PIVOT_POINT_OFFSET)) {
					this->material->get<glm::vec3>(GM_PROPERTY_MESH_PIVOT_POINT_OFFSET) = new_value;
				}
			}
		});

	slots.connect(owner->property_added(), [this](std::shared_ptr<IProperty> property) {
		for (auto &pair : uniforms_for_render_pass)
		{
			pair.second.connect(*property, std::string(), true);
		}
	});
}

Renderable::~Renderable()
{
	render_system->remove_renderable(this);
}

void Renderable::set_up_uniform_listeners()
{
	uniforms_for_render_pass.clear();

	for (const auto &pass_config_pair : material->get_render_pass_configs())
	{
		PropertyToUniformConnector &uni_pass = uniforms_for_render_pass.emplace(std::make_pair(pass_config_pair.first, pass_config_pair.second.create_derivative())).first->second;

		for (const auto &property_pair : owner->get_properties())
		{
			uni_pass.connect(*property_pair.second, std::string(), true);
		}
	}
}

void Renderable::update_uniforms(Camera &camera, const std::string &render_pass_name) { 
	// Let's only actually update the normal matrix value based on world and view matrix if the shader use it.
	if (material->has_property(GM_PROPERTY_NORMAL_MATRIX)) {
		update_normal_matrix(camera.get_view_matrix());
	}

	uniforms_for_render_pass[render_pass_name].update_uniforms(); 
}

void Renderable::update_normal_matrix(const glm::mat4 &view_matrix) {
	normal_matrix_property = make_normal_matrix(view_matrix);
}

glm::mat3 Renderable::make_normal_matrix(const glm::mat4 &view_matrix) const {
	return glm::inverseTranspose(glm::mat3(view_matrix * world_matrix_property.get()));
}

void Renderable::set_render_layers(unsigned int new_render_layers) {
	render_system->remove_renderable(this);

	render_layers = new_render_layers;

	render_system->add_renderable(this);
}

} // namespace Framework
} // namespace GM
