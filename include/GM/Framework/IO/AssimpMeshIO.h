#pragma once

#include "IMeshIO.h"

namespace GM {
namespace Framework {

class AssimpMeshIO : public IMeshIO {
public:
	AssimpMeshIO();
	virtual ~AssimpMeshIO();

	Core::VaoLayout load(const std::string &file_name) override;
};

} // namespace Framework
} // namespace GM
