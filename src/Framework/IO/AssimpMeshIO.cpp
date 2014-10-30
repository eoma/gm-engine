#include "GM/Framework/IO/AssimpMeshIO.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/VaoManager.h"
#include "GM/Framework/Utilities/Mesh.h"

#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/VaoLayout.h"
#include "GM/Core/Utilities/RenderCommand.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#include <ClanLib/core.h>

#include <cctype>
#include <algorithm>

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

MeshPtr AssimpMeshIO::load(const std::string &mesh_name, const std::string &file_name, unsigned int mesh_index, const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager)
{
	auto scene = importer->ReadFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	if (scene == nullptr)
	{
		throw clan::Exception(clan::string_format("Unable to locate (%1) for (%2)", file_name, mesh_name));
	}

	if (scene->mNumMeshes <= mesh_index)
		throw clan::Exception(clan::string_format("The mesh index (%1) was out of bounds for mesh %2.", mesh_index, mesh_name));

	auto scene_mesh = scene->mMeshes[mesh_index];

	unsigned int stride = 0;
	std::vector<Core::VertexAttribute> interleaved_spec;
	Core::VaoLayout vao_layout;
	Core::RenderCommand render_command;

	// vectors to store bounds position and size in.
	glm::vec3 mesh_bounds_pos = glm::vec3(0);
	glm::vec3 mesh_bounds_size = glm::vec3(0);

	if (scene_mesh->HasPositions() && scene_mesh->mNumVertices > 0)
	{
		mesh_bounds_pos = glm::vec3(scene_mesh->mVertices[0].x, scene_mesh->mVertices[0].y, scene_mesh->mVertices[0].z);
		mesh_bounds_size = mesh_bounds_pos;
	}

	//
	// Set up vertices
	//

	// Set up the interleaved vertex attributes
	if (scene_mesh->HasPositions())
	{
		stride += sizeof(glm::vec3);
		interleaved_spec.push_back(Core::VaoArg<glm::vec3>(Core::ShaderConstants::Position));
	}

	if (scene_mesh->HasNormals())
	{
		stride += sizeof(glm::vec3);
		interleaved_spec.push_back(Core::VaoArg<glm::vec3>(Core::ShaderConstants::Normal));
	}

	if (scene_mesh->HasTextureCoords(0))
	{
		stride += sizeof(glm::vec2);
		interleaved_spec.push_back(Core::VaoArg<glm::vec2>(Core::ShaderConstants::TexCoord));
	}

	if (scene_mesh->HasTangentsAndBitangents())
	{
		stride += 2*sizeof(glm::vec3);
		interleaved_spec.push_back(Core::VaoArg<glm::vec3>(Core::ShaderConstants::Tangent));
		interleaved_spec.push_back(Core::VaoArg<glm::vec3>(Core::ShaderConstants::Bitangent));
	}

	std::vector<float> vertices;
	vertices.reserve(scene_mesh->mNumVertices * stride / sizeof(float)); // This is how many floats we have

	for (unsigned int i = 0; i < scene_mesh->mNumVertices; i++) {
		// This may be made into a lambda for BuffeOperations::unsafe_upload
		if (scene_mesh->HasPositions())
		{
			auto position = scene_mesh->mVertices[i];
			vertices.push_back(position.x);
			vertices.push_back(position.y);
			vertices.push_back(position.z);

			// check for new min-max component in vertex, and update bounds.
			check_if_new_min_or_max_vertex(glm::vec3(position.x, position.y, position.z),
			                               mesh_bounds_pos, mesh_bounds_size);
		}

		if (scene_mesh->HasNormals())
		{
			auto normal = scene_mesh->mNormals[i];
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);
		}

		if (scene_mesh->HasTextureCoords(0))
		{
			auto texcoord = scene_mesh->mTextureCoords[0][i];
			vertices.push_back(texcoord.x);
			vertices.push_back(texcoord.y);
		}

		if (scene_mesh->HasTangentsAndBitangents())
		{
			auto tangent = scene_mesh->mTangents[i];
			vertices.push_back(tangent.x);
			vertices.push_back(tangent.y);
			vertices.push_back(tangent.z);

			auto bitangent = scene_mesh->mBitangents[i];
			vertices.push_back(bitangent.x);
			vertices.push_back(bitangent.y);
			vertices.push_back(bitangent.z);
		}
	}

	auto vertex_allocation = buffer_manager->allocate(vertices.size()*stride, stride);
	vertex_allocation.upload(vertices);

	vao_layout
		.for_buffer(vertex_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind_interleaved(0, 0, interleaved_spec);
	
	render_command.set_draw_mode(GL_TRIANGLES);
	render_command.set_vertices(vertex_allocation, scene_mesh->mNumVertices, stride);

	//
	// Set up indices
	//

	if (scene_mesh->HasFaces()) {
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < scene_mesh->mNumFaces; i++) {
			auto face = scene_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		auto index_allocation = buffer_manager->allocate_and_upload(indices);

		vao_layout
			.for_buffer(index_allocation)
				.use_as(GL_ELEMENT_ARRAY_BUFFER);

		render_command.set_indices(index_allocation, indices);
	}

	// Ensure that mesh_bounds_size is the distance from mesh_bounds_pos to upper-right-back-corner of the bounds
	mesh_bounds_size = glm::vec3(
	                             mesh_bounds_size.x - mesh_bounds_pos.x,
	                             mesh_bounds_size.y - mesh_bounds_pos.y,
	                             mesh_bounds_size.z - mesh_bounds_pos.z
	                            );

	return std::make_shared<Mesh>(render_command, vao_layout, vao_manager, mesh_name, mesh_bounds_pos, mesh_bounds_size);
}

// Check if this vertex has components that are greater or less than or more than the corresponding components in
// bounds_pos and bounds_size, respectively, and if so, set the relevant component as new component in bounds_pos
// or bounds_size.
void AssimpMeshIO::check_if_new_min_or_max_vertex(const glm::vec3 &vertex, glm::vec3 &bounds_pos, glm::vec3 &bounds_size) {

	// loop through the vectors
	for(unsigned int i = 0; i < 3; i++) {

		// if vertex[i] has a greater value than bounds_size[i], set bounds_size[i] = vertex[i]
		if(vertex[i] > bounds_size[i]) {
			bounds_size[i] = vertex[i];
		}
		// if vertex[i] has lower value than bounds_pos[i], set bounds_pos[i] = vertex[i]
		if(vertex[i] < bounds_pos[i]) {
			bounds_pos[i] = vertex[i];
		}
	}
}

} // namespace Framework
} // namespace GM
