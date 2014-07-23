#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Core/GL/VertexArrayObject.h"

#include <GL/gl3w.h>

namespace GM {
namespace Framework {

using namespace Core;

VaoManager::VaoManager()
: vaos()
{
}

VaoManager::~VaoManager()
{
}

Core::VertexArrayObjectPtr VaoManager::get_vao_for(const VaoLayout &layout)
{
	auto iter = vaos.find(layout);

	if (iter == vaos.end())
	{
		auto vao = build_vao(layout);
		iter = vaos.insert(std::make_pair(layout, vao)).first;
	}

	return iter->second;
}

Core::VertexArrayObjectPtr VaoManager::build_vao(const VaoLayout &layout)
{
	auto vao = std::make_shared<VertexArrayObject>();
	vao->bind();

	for (const BufferUse &used_buffer : layout.get_used_buffers())
	{
		if (used_buffer.type != GL_ARRAY_BUFFER)
		{
			// activate used_buffer.name with type used_buffer.type
		}
	}

	for (const BufferVertexAttribDefinition &def : layout.get_definitions())
	{
		// activate def.buffer_name
		// bind vertex attrib with all arguments in def
	}

	vao->unbind();

	return vao;
}

} // namespace Framework
} // namespace GM
