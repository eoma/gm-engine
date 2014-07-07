#include "GM/Framework/Utilities/VaoManager.h"

// remove when intriducing gl
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 1
#endif

namespace GM {
namespace Framework {

VaoManager::VaoManager()
{
}

VaoManager::~VaoManager()
{
	// clean up
	for (auto &layout_and_vao : vaos) {
		// Release vao stored in layoutAndVao.second
	}
}

unsigned int VaoManager::get_vao_for(const VaoLayout &layout)
{
	auto iter = vaos.find(layout);

	if (iter == vaos.end())
	{
		unsigned int vao = build_vao(layout);
		iter = vaos.insert(std::make_pair(layout, vao)).first;
	}

	return iter->second;
}

unsigned int VaoManager::build_vao(const VaoLayout &layout)
{
	unsigned int vao = 0;

	// glGenVertexArrays(&vao, 0); or something similar

	for (const BufferUse &buffer_use : layout.get_buffer_use())
	{
		if (buffer_use.type != GL_ARRAY_BUFFER)
		{
			// activate buffer_use.name with type buffer_use.type
		}
	}

	for (const BufferVertexAttribDefinition &def : layout.get_definitions())
	{
		// activate def.buffer_name
		// bind vertex attrib with all arguments in def
	}

	return vao;
}

} // namespace Framework
} // namespace GM
