#pragma once

#include "IMeshIO.h"
#include <glm/glm.hpp>

namespace Assimp {
	class Importer;
}

namespace GM {
namespace Framework {
class AssimpMeshIO : public IMeshIO {
public:
	AssimpMeshIO();
	virtual ~AssimpMeshIO();

	MeshPtr load(const std::string &mesh_name, const std::string &file_name, unsigned int mesh_index, const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;

private:
	void check_if_new_min_or_max_vertex(const glm::vec3 &vertex, glm::vec3 &bounds_pos, glm::vec3 &bounds_size);
	Assimp::Importer *importer;
};

} // namespace Framework
} // namespace GM
