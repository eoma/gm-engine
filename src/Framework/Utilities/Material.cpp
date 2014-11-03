#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Components/Light.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"

#include <glm/glm.hpp>

#include <iostream>

namespace GM {
namespace Framework {

	Material::Material(const TextureManagerPtr &texture_manager, const Core::ShaderPtr &standard_render_pass_shader, const std::string &name)
: name(name)
, texture_manager(texture_manager)
, used_uniforms()
{
	set_render_pass(standard_render_pass_shader);

	property_added_slot = sign_PropertyAdded.connect([this](IPropertyPtr p) {
		if (IProperty::is_type<std::string>(p)) {
			auto texture_property_name = p->get_name().substr(0, p->get_name().find_last_of("_name") - 4);
			auto property = get<std::string>(p->get_name());
			
			// make sure this texture actually exist on this material
			if (has_property(texture_property_name)) {
				auto texture_property = get<Core::TexturePtr>(texture_property_name);
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
	return render_pass_configs.at(render_pass_name).shader;
}

void Material::set_render_pass(const Core::ShaderPtr &shader, const std::string &render_pass_name) {
	render_pass_configs[render_pass_name] = RenderPassConfig(shader);
	set_up_uniforms(render_pass_name);
}

//

const Material::RenderPassConfig &Material::get_render_pass_config(const std::string &render_pass_name) const {
	return render_pass_configs.at(render_pass_name);
}

const std::map<std::string, Material::RenderPassConfig> &Material::get_render_pass_configs() const {
	return render_pass_configs;
}

//

void Material::update_uniforms(Camera * camera, const std::vector<Light *> &lights, const std::string &render_pass_name) {
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
	render_pass_configs[render_pass_name].update_uniforms_signal(); 

	// If this material is using tesselation, this property must be set with the amount of patch vertices to generate.
	if (patch_vertices_property > 0) {
		glPatchParameteri(GL_PATCH_VERTICES, patch_vertices_property);
	}
}

//

void Material::set_up_uniforms(const std::string &render_pass_name) {
	RenderPassConfig &settings = render_pass_configs[render_pass_name];
	Core::ShaderPtr &shader = settings.shader;

	settings.unused_uniforms.reserve(shader->get_uniform_infos().size());
	for (const Core::ShaderVariableInfo &info : shader->get_uniform_infos())
	{
		settings.unused_uniforms.push_back(info.name);

		switch (info.type)
		{
		case GL_BOOL:
			{
				add_sleeping_uniform<bool>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}
		case GL_BYTE:
			{
				add_sleeping_uniform<char>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_BYTE:
			{
				add_sleeping_uniform<unsigned char>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_SHORT:
			{
				add_sleeping_uniform<short>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_SHORT:
			{
				add_sleeping_uniform<unsigned short>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_INT:
			{
				add_sleeping_uniform<int>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT:
			{
				add_sleeping_uniform<unsigned int>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT:
			{
				add_sleeping_uniform<float>(render_pass_name, info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_VEC2:
			{
				add_sleeping_uniform<glm::vec2>(render_pass_name, info.name, glm::vec2(), shader->get_handle(), info.location);
				break;
			}

		case GL_INT_VEC2:
			{
				add_sleeping_uniform<glm::ivec2>(render_pass_name, info.name, glm::ivec2(), shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC2:
			{
				add_sleeping_uniform<glm::uvec2>(render_pass_name, info.name, glm::uvec2(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_VEC3:
			{
				add_sleeping_uniform<glm::vec3>(render_pass_name, info.name, glm::vec3(), shader->get_handle(), info.location);
				break;
			}

		case GL_INT_VEC3:
			{
				add_sleeping_uniform<glm::ivec3>(render_pass_name, info.name, glm::ivec3(), shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC3:
			{
				add_sleeping_uniform<glm::uvec3>(render_pass_name, info.name, glm::uvec3(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_VEC4:
			{
				add_sleeping_uniform<glm::vec4>(render_pass_name, info.name, glm::vec4(), shader->get_handle(), info.location);
				break;
			}

		case GL_INT_VEC4:
			{
				add_sleeping_uniform<glm::ivec4>(render_pass_name, info.name, glm::ivec4(), shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC4:
			{
				add_sleeping_uniform<glm::uvec4>(render_pass_name, info.name, glm::uvec4(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_MAT2:
			{
				add_sleeping_uniform<glm::mat2>(render_pass_name, info.name, glm::mat2(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_MAT3:
			{
				add_sleeping_uniform<glm::mat3>(render_pass_name, info.name, glm::mat3(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_MAT4:
			{
				add_sleeping_uniform<glm::mat4>(render_pass_name, info.name, glm::mat4(), shader->get_handle(), info.location);
				break;
			}

		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			{
				add_sleeping_uniform<Core::TexturePtr>(render_pass_name, info.name, nullptr, shader->get_handle(), info.location);
				break;
			}

		default:
			{
				clan::StringFormat message("Uknown/unhandled type (%1) for uniform (%2) with length (%3)");
				message.set_arg(1, info.type);
				message.set_arg(2, info.name);
				message.set_arg(3, info.size);

				std::cerr << message.get_result();
				break;
			}
		};
	}
}

template <class T>
void Material::add_sleeping_uniform(const std::string &render_pass_name, const std::string &name, const T &default_value, const unsigned int program, const int uniform_location) {
	bool prop_exists_and_is_used = (has_property(name) && 
		(std::find(used_uniforms.begin(), used_uniforms.end(), name) != used_uniforms.end()));
	Property<T> prop = add<T>(name, default_value);

	std::function<void()> update_lambda = uniform_lambda_maker(render_pass_name, program, uniform_location, prop);

	if (prop_exists_and_is_used) {
		// No need to listen for changes, it is already in use
		auto &cfg = render_pass_configs[render_pass_name];
		cfg.update_uniform_slots.connect(cfg.update_uniforms_signal, update_lambda);
	} else {
		// Set up a callback for when the property is written to for the first time (after we added it)
		render_pass_configs[render_pass_name].wait_for_first_write_slots.emplace(
			name,

			// When the property changes, we will make update_lambda execute when cfg.update_uniforms_signal is triggered
			prop.value_changed().connect([this, name, render_pass_name, update_lambda](const T &/*old_v*/, const T &/*new_v*/) {
				auto &cfg = render_pass_configs[render_pass_name];
				cfg.unused_uniforms.erase(std::find(cfg.unused_uniforms.begin(), cfg.unused_uniforms.end(), name));

				// The property's been set, erase first write callback slot
				cfg.wait_for_first_write_slots.erase(name);

				// Update local list of used uniforms
				cfg.used_uniforms.push_back(name);

				if (std::find(used_uniforms.begin(), used_uniforms.end(), name) == used_uniforms.end()) {
					// Update the global list of uniforms in this material
					used_uniforms.push_back(name); 
				}

				cfg.update_uniform_slots.connect(cfg.update_uniforms_signal, update_lambda);
			})
		);
	}
}

// Special case updater for textures
template <>
std::function<void()> Material::uniform_lambda_maker<Core::TexturePtr>(
	const std::string &render_pass_name,
	const unsigned int program,
	const int uniform_location,
	const Property<Core::TexturePtr> &prop)
{
	int texture_unit = render_pass_configs[render_pass_name].compute_available_texture_unit();

	return [program, uniform_location, prop, texture_unit]() {
		if (prop.get() == nullptr) {
			return;
		}

		glActiveTexture(GL_TEXTURE0 + texture_unit);
		prop.get()->bind();

		Core::update_uniform(program, uniform_location, prop.get(), texture_unit);
	};
}

// General case
template <class T>
std::function<void()> Material::uniform_lambda_maker(
	const std::string &/*render_pass_name*/,
	const unsigned int program,
	const int uniform_location,
	const Property<T> &prop)
{
	return [program, uniform_location, prop]() {
		Core::update_uniform(program, uniform_location, (T)prop.get());
	};
}

} // namespace Framework
} // namespace GM
