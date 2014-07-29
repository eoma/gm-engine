#include "GM/Framework/Managers/MaterialManager.h"

#include "ClanLib/core.h"

namespace GM {
namespace Framework {

MaterialManager::MaterialManager() {

}

MaterialManager::~MaterialManager() {

}

bool MaterialManager::contains(const std::string &name) const
{
	return (materials.end() !=  materials.find(name));
}

void MaterialManager::add(const std::string &name, const MaterialPtr &material)
{
	auto iter = materials.find(name);
	if (iter == materials.end())
	{
		materials.insert(std::make_pair(name, material));
	}
	else
	{
		clan::StringFormat message = clan::StringFormat("Material manager already contains a material by name (%1)");
		message.set_arg(1, name);
		throw clan::Exception(message.get_result());
	}
}

MaterialPtr MaterialManager::get(const std::string &name) const
{
	MaterialPtr material = nullptr;

	auto iter = materials.find(name);
	if (iter != materials.end())
	{
		material = iter->second;
	}

	return material;
}

} // namespace Framework
} // namespace GM
