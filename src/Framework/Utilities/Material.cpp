#include "GM/Framework/Utilities/Material.h"

namespace GM {
namespace Framework {

Material::Material(const Core::ShaderPtr &shader, const std::string &name)
: name(name)
, shader(shader)
, property_uniform_pairs()
{
	// Set up uniforms from shader
}

void Material::add_texture(const std::string &name, const Core::TexturePtr &texture)
{
	add_uniform<Core::TexturePtr>(name, texture);
}

} // namespace Framework
} // namespace GM
