#include "GM/Framework/IO/AssimpMeshIO.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/VaoManager.h"
#include "GM/Framework/Utilities/Mesh.h"

#include "GM/Core/Utilities/VaoLayout.h"
#include "GM/Core/Utilities/RenderCommand.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#include <ClanLib/core.h>

#include <cctype>
#include <algorithm>

#define POSITION 0
#define NORMAL 1
#define TEXCOORD 2

namespace GM {
namespace Framework {

AssimpMeshIO::AssimpMeshIO()
: importer(new Assimp::Importer())
{
}

AssimpMeshIO::~AssimpMeshIO()
{
	delete importer;
}

MeshPtr AssimpMeshIO::load(const std::string &mesh_name, const std::string &file_name, int mesh_index, const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager)
{
	auto scene = importer->ReadFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene->mNumMeshes <= mesh_index)
		throw clan::Exception(clan::string_format("The mesh index (%1) was out of bounds for mesh %2.", mesh_index, mesh_name));

	auto scene_mesh = scene->mMeshes[mesh_index];

	struct MyVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
		MyVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord) : position(position), normal(normal), texcoord(texcoord) {}
	};
	std::vector<MyVertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < scene_mesh->mNumFaces; i++) {
		auto face = scene_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			auto position = scene_mesh->mVertices[face.mIndices[j]];
			auto normal = scene_mesh->HasNormals() ? scene_mesh->mNormals[face.mIndices[j]] : aiVector3D(1, 1, 1);
			auto texcoord = scene_mesh->mTextureCoords[0][face.mIndices[j]];

			vertices.push_back(MyVertex(
				glm::vec3(position.x, position.y, position.z), 
				glm::vec3(normal.x, normal.y, normal.z), 
				glm::vec2(texcoord.x, texcoord.y)
			));

			indices.push_back(face.mIndices[j]);
		}
	}

	auto buffer_allocation = buffer_manager->allocate_and_upload(vertices);
	auto index_allocation = buffer_manager->allocate_and_upload(indices);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(buffer_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind_interleaved(Core::VaoArg<glm::vec3>(POSITION), Core::VaoArg<glm::vec3>(NORMAL), Core::VaoArg<glm::vec2>(TEXCOORD))
		.for_buffer(index_allocation)
			.use_as(GL_ELEMENT_ARRAY_BUFFER)
	;

	auto render_command = Core::RenderCommand(true, vertices.size(), 0, buffer_allocation.offset / sizeof(MyVertex));

	return std::make_shared<Mesh>(mesh_name, render_command, vao_layout, vao_manager);
}

} // namespace Framework
} // namespace GM
