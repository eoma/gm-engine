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

	Core::VaoLayout load(const std::string &file_name) override;

private:
	Assimp::Importer *importer;
};

} // namespace Framework
} // namespace GM
