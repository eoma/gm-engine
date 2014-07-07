#pragma once

#include "../../Core/Utilities/VaoLayout.h"

#include <map>


namespace GM {
namespace Framework {

class VaoManager
{
public:
	VaoManager();
	~VaoManager();

	unsigned int get_vao_for(const Core::VaoLayout &layout);

private:
	unsigned int build_vao(const Core::VaoLayout &layout);

private:
	std::map<Core::VaoLayout, unsigned int> vaos;

};

} // namespace Framework
} // namespace GM
