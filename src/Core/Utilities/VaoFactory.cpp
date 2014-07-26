#include "GM/Core/Utilities/VaoFactory.h"

#include "GM/Core/GL/VertexArrayObject.h"
#include "GM/Core/Utilities/BufferVertexAttribDefinition.h"
#include "GM/Core/Utilities/BufferUse.h"

namespace GM {
namespace Core {

VertexArrayObjectPtr VaoFactory::create(const VaoLayout& vao_layout)
{
	auto vao = std::make_shared<VertexArrayObject>();
	vao->bind();

	for (const BufferUse &used_buffer : vao_layout.get_used_buffers())
	{
		if (used_buffer.type != GL_ARRAY_BUFFER)
		{
			// activate used_buffer.name with type used_buffer.type
			used_buffer.buffer->bind_as(used_buffer.type);
		}
	}

	BufferObjectPtr active_buffer = nullptr;

	for (const BufferVertexAttribDefinition &def : vao_layout.get_definitions())
	{
		if (active_buffer != def.buffer)
		{
			def.buffer->bind(); // or bind_as(GL_ARRAY_BUFFER)?
			active_buffer = def.buffer;
		}

		glVertexAttribPointer(def.index, def.size_per_index, def.data_type, def.normalized, def.stride, reinterpret_cast<const void*>(def.offset));
		glEnableVertexAttribArray(def.index);
		if (def.divisor > 0)
		{
			glVertexAttribDivisor(def.index, def.divisor);
		}
	}

	vao->unbind();

	for (const BufferUse &used_buffer : vao_layout.get_used_buffers())
	{
		// deactivate used_buffer
		used_buffer.buffer->unbind();
	}

	return vao;
}

} // namespace Core
} // namespace GM
