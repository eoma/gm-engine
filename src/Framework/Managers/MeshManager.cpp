#include "GM/Framework/Managers/MeshManager.h"
#include "GM/Framework/Templates/MeshTemplateManager.h"
#include "GM/Framework/Primitives/PrimitivesManager.h"
#include <GM/Framework/IO/IMeshIO.h>
#include <GM/Framework/Utilities/Mesh.h>
#include "ClanLib/core.h"

#include <algorithm>

namespace GM {
namespace Framework {

MeshManager::MeshManager(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager, const IMeshIOPtr &mesh_io)
: buffer_manager(buffer_manager), vao_manager(vao_manager), mesh_io(mesh_io)
{
	template_manager = MeshTemplateManagerPtr(new MeshTemplateManager());
	primitives_manager = PrimitivesManagerPtr(new PrimitivesManager(buffer_manager, vao_manager));
}

MeshManager::~MeshManager() {

}

bool MeshManager::contains(const std::string &name) const
{
	return (meshes.end() != meshes.find(name));
}

void MeshManager::add(const MeshPtr &mesh)
{
	if (!contains(mesh->get_name()))
	{
		meshes.insert(std::make_pair(mesh->get_name(), mesh));
	}
	else
	{
		clan::StringFormat message = clan::StringFormat("Mesh manager already contains a mesh by name (%1)");
		message.set_arg(1, mesh->get_name());
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

MeshPtr MeshManager::get_or_create(const std::string &name) {
	// First, test if the name has been cached.
	auto mesh = get(name);
	if (mesh) {
		return mesh;
	}

	//If not cached, let's see if there is a template description for this name.
	template_manager->get(name, [this, name, &mesh](const MeshTemplateManager::Template &t) {
		mesh = get_or_create(name, t.filename, t.mesh_index);
	});
	if (mesh) {
		return mesh;
	}

	//If no template was generated for this name, maybe it's a primitive?
	mesh = primitives_manager->create(name);
	if (mesh) {
		add(mesh);
		return mesh;
	}

	throw clan::Exception(clan::string_format("Failed to get or create the mesh %1.", name));
}

MeshPtr MeshManager::get_or_create(const std::string &name, const std::string &filename, int mesh_index) {
	// First, test if the name has been cached.
	auto mesh = get(name);
	if (mesh) {
		return mesh;
	}

	bool file_found = false;
	std::string complete_path;

	for (const std::string& path : mesh_paths) {
		if (path.empty()) {
			continue;
		}

		std::string file_path = clan::PathHelp::make_absolute(path, filename);

		file_found = clan::FileHelp::file_exists(file_path);
		if (file_found) {
			complete_path = file_path;
			break;
		}
	}

	if (!file_found) {
		throw clan::Exception(clan::string_format("Mesh (%1) does not exist!", filename));
	}

	mesh = mesh_io->load(name, complete_path, mesh_index, buffer_manager, vao_manager);
	if (mesh) {
		add(mesh);
	}
	else {
		throw clan::Exception(clan::string_format("Failed to get or create the mesh %1.", name));
	}

	return mesh;
}

void MeshManager::add_templates(const std::string &template_filename) {
	template_manager->add_templates(template_filename);
}

void MeshManager::add_primitive(IPrimitivePtr primitive) {
	primitives_manager->add(primitive);
}

} // namespace Framework
} // namespace GM
