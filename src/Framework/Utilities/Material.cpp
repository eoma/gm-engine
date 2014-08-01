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
}

void Material::add_texture(const std::string &name, const Core::TexturePtr &texture)
{
	add_uniform<Core::TexturePtr>(name, texture);
}

} // namespace Framework
} // namespace GM
