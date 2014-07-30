#pragma once

#include <GM/Core/Utilities/VaoLayout.h>

#include <string>
#include <vector>

namespace GM {
namespace Framework {

class IMeshIO {
public:
	virtual ~IMeshIO() {};

	/**
	 * Load mesh from file.
	 */
	virtual Core::VaoLayout load(const std::string &file_name) = 0;
};

} // namespace Framework
} // namespace GM
