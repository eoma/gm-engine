#include "GM/Framework/Managers/MeshManager.h"

#include "ClanLib/core.h"

#include <algorithm>

namespace GM {
namespace Framework {

MeshManager::MeshManager() {

}

MeshManager::~MeshManager() {

}

bool MeshManager::contains(const std::string &name) const
{
	return (meshes.end() != meshes.find(name));
}

void MeshManager::add(const std::string &name, const MeshPtr &mesh)
{
	if (!contains(name))
	{
		meshes.insert(std::make_pair(name, mesh));
	}
	else
	{
		clan::StringFormat message = clan::StringFormat("Mesh manager already contains a mesh by name (%1)");
		message.set_arg(1, name);
		throw clan::Exception(message.get_result());
	}
}

MeshPtr MeshManager::get(const std::string &name) const
{
	MeshPtr mesh = nullptr;

	auto iter = meshes.find(name);
	if (iter != meshes.end())
	{
		mesh = iter->second;
	}

	return mesh;
}

} // namespace Framework
} // namespace GM
