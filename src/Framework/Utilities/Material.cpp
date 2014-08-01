#include "GM/Framework/Utilities/Material.h"

#include "GM/Core/GL/Shader.h"

#include <glm/glm.hpp>

#include <iostream>

namespace GM {
namespace Framework {

Material::Material(const Core::ShaderPtr &shader, const std::string &name)
: name(name)
, shader(shader)
, used_uniforms()
, unused_uniforms()
{
	unused_uniforms.reserve(shader->get_uniform_infos().size());
	//used_uniforms.reserve(shader->get_uniform_infos().size());
	for (const Core::ShaderVariableInfo &info : shader->get_uniform_infos())
	{
		unused_uniforms.push_back(info.name);
	}

	property_added_slot = sign_PropertyAdded.connect([&](IPropertyPtr property) {
		if (IProperty::is_type<char>(property)) {
			auto p = get<char>(property->get_name());
			add_uniform<char>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<unsigned char>(property)) {
			auto p = get<unsigned char>(property->get_name());
			add_uniform<unsigned char>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<short>(property)) {
			auto p = get<short>(property->get_name());
			add_uniform<short>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<unsigned short>(property)) {
			auto p = get<unsigned short>(property->get_name());
			add_uniform<unsigned short>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<int>(property)) {
			auto p = get<int>(property->get_name());
			add_uniform<int>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<unsigned int>(property)) {
			auto p = get<unsigned int>(property->get_name());
			add_uniform<unsigned int>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<float>(property)) {
			auto p = get<float>(property->get_name());
			add_uniform<float>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::vec2>(property)) {
			auto p = get<glm::vec2>(property->get_name());
			add_uniform<glm::vec2>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::ivec2>(property)) {
			auto p = get<glm::vec2>(property->get_name());
			add_uniform<glm::vec2>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::uvec2>(property)) {
			auto p = get<glm::vec2>(property->get_name());
			add_uniform<glm::vec2>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::vec3>(property)) {
			auto p = get<glm::vec3>(property->get_name());
			add_uniform<glm::vec3>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::ivec3>(property)) {
			auto p = get<glm::vec3>(property->get_name());
			add_uniform<glm::vec3>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::uvec3>(property)) {
			auto p = get<glm::vec3>(property->get_name());
			add_uniform<glm::vec3>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::vec4>(property)) {
			auto p = get<glm::vec4>(property->get_name());
			add_uniform<glm::vec4>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::ivec4>(property)) {
			auto p = get<glm::vec4>(property->get_name());
			add_uniform<glm::vec4>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::uvec4>(property)) {
			auto p = get<glm::vec4>(property->get_name());
			add_uniform<glm::vec4>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::mat2>(property)) {
			auto p = get<glm::mat2>(property->get_name());
			add_uniform<glm::mat2>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::mat3>(property)) {
			auto p = get<glm::mat3>(property->get_name());
			add_uniform<glm::mat3>(p.get_name(), p.get());
		}
		else if (IProperty::is_type<glm::mat4>(property)) {
			auto p = get<glm::mat4>(property->get_name());
			add_uniform<glm::mat4>(p.get_name(), p.get());
		}
		/*else if (IProperty::is_type<Core::Texture>(property)) {
			auto p = get<Core::Texture>(property->get_name());
			add_uniform<Core::Texture>(p.get_name(), p.get());
		}*/
	});
}

void Material::add_texture(const std::string &name, const Core::TexturePtr &texture)
{
	add_uniform<Core::TexturePtr>(name, texture);
}

} // namespace Framework
} // namespace GM
