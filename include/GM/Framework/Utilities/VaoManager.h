#pragma once

#include "VaoLayout.h"

#include <map>


namespace GM {
namespace Framework {

class VaoManager
{
public:
	VaoManager();
	~VaoManager();

	unsigned int get_vao_for(const VaoLayout &layout);

private:
	unsigned int build_vao(const VaoLayout &layout);

private:
	std::map<VaoLayout, unsigned int> vaos;

};

} // namespace Framework
} // namespace GM
