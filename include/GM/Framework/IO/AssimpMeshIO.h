#pragma once

#include "IMeshIO.h"

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
	Assimp::Importer *importer;
};

} // namespace Framework
} // namespace GM
