#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Components/Light.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"

#include <glm/glm.hpp>

#include <iostream>

namespace GM {
namespace Framework {

Material::Material(const TextureManagerPtr &texture_manager, const Core::ShaderPtr &standard_render_pass_shader, const std::string &name)
: Material(texture_manager, std::map<std::string, Core::ShaderPtr>{{"standard", standard_render_pass_shader}}, name)
{
}

Material::Material(const TextureManagerPtr &texture_manager, const std::map<std::string, Core::ShaderPtr> &render_pass_shaders, const std::string &name)
: name(name)
, texture_manager(texture_manager)
, used_uniforms()
{
	for (const auto &iter : render_pass_shaders) {
		set_render_pass(iter.second, iter.first);
	}

	property_added_slot = sign_PropertyAdded.connect([this](IPropertyPtr p) {
		if (IProperty::is_type<std::string>(p)) {
			auto texture_property_name = p->get_name().substr(0, p->get_name().find_last_of("_name") - 4);
			auto property = get<std::string>(p->get_name());
			
			// make sure this texture actually exist on this material
			if (has_property(texture_property_name)) {
				auto texture_property = add<Core::TexturePtr>(texture_property_name, nullptr);
				texture_property = this->texture_manager->get_or_create(property.get());
			}
		}
	});

	patch_vertices_property = add<int>(GM_PROPERTY_PATCH_VERTICES, 0);
}

//

bool Material::has_render_pass(const std::string &render_pass_name) const {
	return render_pass_configs.find(render_pass_name) != render_pass_configs.end();
}

const Core::ShaderPtr &Material::get_render_pass(const std::string &render_pass_name) const {
	return render_pass_configs.at(render_pass_name).get_shader();
}

void Material::set_render_pass(const Core::ShaderPtr &shader, const std::string &render_pass_name) {
	if (shader == nullptr)
	{
		return;
	}

	render_pass_configs[render_pass_name] = PropertyToUniformConnector(shader);
	render_pass_configs[render_pass_name].connect_property_container(*this);
}

//

const PropertyToUniformConnector &Material::get_render_pass_config(const std::string &render_pass_name) const {
	return render_pass_configs.at(render_pass_name);
}

const std::map<std::string, PropertyToUniformConnector> &Material::get_render_pass_configs() const {
	return render_pass_configs;
}

//

void Material::update_uniforms(const Camera &camera, const std::vector<Light *> &lights, const std::string &render_pass_name) {
	if (has_property("light_count")) {
		get<int>("light_count") = (int)lights.size();

		for (unsigned int i = 0; i < lights.size(); i++) {
			auto light_in_array_name = clan::string_format("light[%1]", i);
			auto light_position_in_viewspace_name = clan::string_format("%1.position_in_viewspace", light_in_array_name);
			if (has_property(light_position_in_viewspace_name)) {
				auto light_radius_name = clan::string_format("%1.radius", light_in_array_name);
				auto light_material_color_diffuse_name = clan::string_format("%1.material_color_diffuse", light_in_array_name);
				auto light_material_color_specular_name = clan::string_format("%1.material_color_specular", light_in_array_name);
				auto light_material_color_ambient_name = clan::string_format("%1.material_color_ambient", light_in_array_name);
				auto light_activated_name = clan::string_format("%1.activated", light_in_array_name);

				auto light = lights[i];
				get<glm::vec3>(light_position_in_viewspace_name) = light->get_position_in_viewspace(camera);

				if (has_property(light_radius_name)) {
					get<float>(light_radius_name) = light->get_radius();
				}
				if (has_property(light_material_color_diffuse_name)) {
					get<glm::vec3>(light_material_color_diffuse_name) = light->get_material_color_diffuse();
				}
				if (has_property(light_material_color_specular_name)) {
					get<glm::vec3>(light_material_color_specular_name) = light->get_material_color_specular();
				}
				if (has_property(light_material_color_ambient_name)) {
					get<glm::vec3>(light_material_color_ambient_name) = light->get_material_color_ambient();
				}
				if (has_property(light_activated_name)) {
				    get<bool>(light_activated_name) = light->get_activated();
				}
			}
		}
	}

	if (has_property(GM_PROPERTY_PROJECTION_MATRIX)) {
		get<glm::mat4>(GM_PROPERTY_PROJECTION_MATRIX) = camera.get_projection_matrix();
	}

	if (has_property(GM_PROPERTY_VIEW_MATRIX)) {
		get<glm::mat4>(GM_PROPERTY_VIEW_MATRIX) = camera.get_view_matrix();
	}

	render_pass_configs[render_pass_name].update_uniforms(); 

	// If this material is using tesselation, this property must be set with the amount of patch vertices to generate.
	if (patch_vertices_property > 0) {
		glPatchParameteri(GL_PATCH_VERTICES, patch_vertices_property);
	}
}

} // namespace Framework
} // namespace GM
