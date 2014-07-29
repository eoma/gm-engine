#include "GM/Framework/Utilities/Material.h"

#include "GM/Core/GL/Shader.h"

#include <glm/glm.hpp>

#include <iostream>

namespace GM {
namespace Framework {

Material::Material(const Core::ShaderPtr &shader, const std::string &name)
: name(name)
, shader(shader)
, property_uniform_pairs()
{
	// Set up uniforms from shader
	for (const Core::ShaderVariableInfo &info : shader->get_uniform_info())
	{
		switch (info.type)
		{
		case GL_BYTE:
			{
				add_uniform<char>(info.name, 0, info.location);
				break;
			}

		case GL_UNSIGNED_BYTE:
			{
				add_uniform<unsigned char>(info.name, 0, info.location);
				break;
			}

		case GL_SHORT:
			{
				add_uniform<short>(info.name, 0, info.location);
				break;
			}

		case GL_UNSIGNED_SHORT:
			{
				add_uniform<unsigned short>(info.name, 0, info.location);
				break;
			}

		case GL_INT:
			{
				add_uniform<int>(info.name, 0, info.location);
				break;
			}

		case GL_UNSIGNED_INT:
			{
				add_uniform<unsigned int>(info.name, 0, info.location);
				break;
			}

		case GL_FLOAT:
			{
				add_uniform<float>(info.name, 0, info.location);
				break;
			}

		case GL_FLOAT_VEC2:
			{
				add_uniform<glm::vec2>(info.name, glm::vec2(), info.location);
				break;
			}

		case GL_INT_VEC2:
			{
				add_uniform<glm::ivec2>(info.name, glm::ivec2(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC2:
			{
				add_uniform<glm::uvec2>(info.name, glm::uvec2(), info.location);
				break;
			}

		case GL_FLOAT_VEC3:
			{
				add_uniform<glm::vec3>(info.name, glm::vec3(), info.location);
				break;
			}

		case GL_INT_VEC3:
			{
				add_uniform<glm::ivec3>(info.name, glm::ivec3(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC3:
			{
				add_uniform<glm::uvec3>(info.name, glm::uvec3(), info.location);
				break;
			}

		case GL_FLOAT_VEC4:
			{
				add_uniform<glm::vec4>(info.name, glm::vec4(), info.location);
				break;
			}

		case GL_INT_VEC4:
			{
				add_uniform<glm::ivec4>(info.name, glm::ivec4(), info.location);
				break;
			}

		case GL_UNSIGNED_INT_VEC4:
			{
				add_uniform<glm::uvec4>(info.name, glm::uvec4(), info.location);
				break;
			}

		case GL_FLOAT_MAT2:
			{
				add_uniform<glm::mat2>(info.name, glm::mat2(), info.location);
				break;
			}

		case GL_FLOAT_MAT3:
			{
				add_uniform<glm::mat3>(info.name, glm::mat3(), info.location);
				break;
			}

		case GL_FLOAT_MAT4:
			{
				add_uniform<glm::mat4>(info.name, glm::mat4(), info.location);
				break;
			}

		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			{
				add_uniform<Core::TexturePtr>(info.name, nullptr, info.location);
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
		}
	}
}

void Material::add_texture(const std::string &name, const Core::TexturePtr &texture)
{
	add_uniform<Core::TexturePtr>(name, texture);
}

} // namespace Framework
} // namespace GM
