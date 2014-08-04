#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"

#include <glm/glm.hpp>

#include <iostream>

namespace GM {
namespace Framework {

	Material::Material(const TextureManagerPtr &texture_manager, const Core::ShaderPtr &shader, const std::string &name)
: name(name)
, shader(shader)
, texture_manager(texture_manager)
, used_uniforms()
, unused_uniforms()
{
	unused_uniforms.reserve(shader->get_uniform_infos().size());
	//used_uniforms.reserve(shader->get_uniform_infos().size());
	for (const Core::ShaderVariableInfo &info : shader->get_uniform_infos())
	{
		unused_uniforms.push_back(info.name);

		switch (info.type)
		{
		case GL_BYTE:
			{
				add_sleeping_uniform<char>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_BYTE:
			{
				add_sleeping_uniform<unsigned char>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_SHORT:
			{
				add_sleeping_uniform<short>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_SHORT:
			{
				add_sleeping_uniform<unsigned short>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_INT:
			{
				add_sleeping_uniform<int>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT:
			{
				add_sleeping_uniform<unsigned int>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT:
			{
				add_sleeping_uniform<float>(info.name, 0, shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_VEC2:
			{
				add_sleeping_uniform<glm::vec2>(info.name, glm::vec2(), shader->get_handle(), info.location);
				break;
			}

		case GL_INT_VEC2:
			{
				add_sleeping_uniform<glm::ivec2>(info.name, glm::ivec2(), shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC2:
			{
				add_sleeping_uniform<glm::uvec2>(info.name, glm::uvec2(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_VEC3:
			{
				add_sleeping_uniform<glm::vec3>(info.name, glm::vec3(), shader->get_handle(), info.location);
				break;
			}

		case GL_INT_VEC3:
			{
				add_sleeping_uniform<glm::ivec3>(info.name, glm::ivec3(), shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC3:
			{
				add_sleeping_uniform<glm::uvec3>(info.name, glm::uvec3(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_VEC4:
			{
				add_sleeping_uniform<glm::vec4>(info.name, glm::vec4(), shader->get_handle(), info.location);
				break;
			}

		case GL_INT_VEC4:
			{
				add_sleeping_uniform<glm::ivec4>(info.name, glm::ivec4(), shader->get_handle(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC4:
			{
				add_sleeping_uniform<glm::uvec4>(info.name, glm::uvec4(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_MAT2:
			{
				add_sleeping_uniform<glm::mat2>(info.name, glm::mat2(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_MAT3:
			{
				add_sleeping_uniform<glm::mat3>(info.name, glm::mat3(), shader->get_handle(), info.location);
				break;
			}

		case GL_FLOAT_MAT4:
			{
				add_sleeping_uniform<glm::mat4>(info.name, glm::mat4(), shader->get_handle(), info.location);
				break;
			}

		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			{
				add_sleeping_uniform<Core::TexturePtr>(info.name, nullptr, shader->get_handle(), info.location);
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

	property_added_slot = sign_PropertyAdded.connect([this](IPropertyPtr p) {
		if (IProperty::is_type<std::string>(p)) {
			auto texture_property_name = p->get_name().substr(0, p->get_name().find_last_of("_name") - 4);
			auto property = get<std::string>(p->get_name());
			
			// make sure this texture actually exist on this material
			if (has_property(texture_property_name)) {
				auto texture_property = get<Core::TexturePtr>(texture_property_name);
				texture_property = this->texture_manager->get_or_create(property.get());

				auto info = this->shader->get_uniform_info(texture_property_name);
				textures.push_back(Texture(texture_property, this->shader->get_handle(), info.location));
			}
			// FIXME: If we throw here, we can't set a texture in json/from code when the shader don't support it... might be overkill?
			/*else {
				throw clan::Exception(clan::string_format("Failed to find a texture property with name %1 on material %2", texture_property_name, get_name()));
			}*/
		}
	});
}

void Material::bind_textures() const {
	for (int i = 0; i < (int)textures.size(); i++) {
		auto &texture = textures[i];

		// FIXME: Should this perhaps be set inside bind, and let bind accept the texture unit?
		glActiveTexture(GL_TEXTURE0 + i);
		texture.texture.get()->bind();

		Core::update_uniform(texture.program, texture.location, texture.texture, i);
	}
}

} // namespace Framework
} // namespace GM
