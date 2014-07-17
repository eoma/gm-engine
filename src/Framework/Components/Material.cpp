#include "GM/Framework/Components/Material.h"

namespace GM {
namespace Framework {

Material::Material(const EntityPtr &owner, const Core::ShaderPtr &shader, const std::string &name)
: Component<Material>(owner, name)
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
