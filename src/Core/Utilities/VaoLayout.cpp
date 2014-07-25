#include "GM/Core/Utilities/VaoLayout.h"

#include <GL/gl3w.h>

#include <algorithm>

namespace GM {
namespace Core {

VaoLayout::VaoLayout()
: active_buffer(nullptr)
, active_type(0)
, definitions()
, used_buffers()
{
}

VaoLayout::~VaoLayout()
{
}

VaoLayout &VaoLayout::for_buffer(const BufferObjectPtr &buffer)
{
	if (buffer == nullptr)
	{
		throw clan::Exception("Can not bind to buffer 0");
	}

	active_buffer = buffer;
	active_type = 0;

	return *this;
}

VaoLayout &VaoLayout::for_buffer(const BufferAllocation &buffer_allocation)
{
	return for_buffer(buffer_allocation.buffer);
}
VaoLayout &VaoLayout::use_as(const unsigned int buffer_type) {
	if (active_buffer == nullptr || buffer_type == 0)
	{
		throw clan::Exception("Can't use a 0-buffer or a buffer bound as nothing");
	}

	active_type = buffer_type;

	// We can register the buffer as used, and also with what type

	if (active_type == GL_ARRAY_BUFFER) // We can use multiple vertex buffers for vertex attributes
	{
		BufferUse buffer_use;
		buffer_use.buffer = active_buffer;
		buffer_use.type = active_type;

		auto iter = std::find(used_buffers.begin(), used_buffers.end(), buffer_use);

		if (iter == used_buffers.end())
		{
			used_buffers.push_back(buffer_use);
			std::sort(used_buffers.begin(), used_buffers.end());
		}
	}
	else
	{
		// We can only bind one buffer per type that is not GL_ARRAY_BUFFER
		auto iter = std::find_if(used_buffers.begin(), used_buffers.end(),
			[this](const BufferUse &buffer_use) { return buffer_use.type == active_type; });

		if (iter == used_buffers.end())
		{
			BufferUse buffer_use;
			buffer_use.buffer = active_buffer;
			buffer_use.type = active_type;

			used_buffers.push_back(buffer_use);
			std::sort(used_buffers.begin(), used_buffers.end());
		}
	}

	return *this;
}

VaoLayout &VaoLayout::bind(
	const unsigned int index,
	const unsigned int num_of_type,
	const unsigned int data_type,
	const bool normalized,
	const unsigned int stride,
	const unsigned int offset,
	const unsigned int divisor)
{

	// Can only bind if active_type is a vertex buffer

	if (active_type != GL_ARRAY_BUFFER)
	{
		// Silently ignore or throw exception?
		throw clan::Exception("Can only bind vertex attributes if buffer is GL_ARRAY_BUFFER!");
	}

	BufferVertexAttribDefinition def;

	def.buffer = active_buffer;
	def.index = index;
	def.size_per_index = num_of_type;
	def.data_type = data_type;
	def.normalized = normalized;
	def.stride = stride;
	def.offset = offset;
	def.divisor = divisor;

	if (!std::binary_search(definitions.begin(), definitions.end(), def))
	{
		// No such definitions exists
		definitions.push_back(def);
		std::sort(definitions.begin(), definitions.end());
	}

	return *this;
}

VaoLayout &VaoLayout::bind_interleaved(const unsigned int offset, const unsigned int divisor, const std::vector<VertexAttribute> &interleaved_attribs)
{
	unsigned int stride = 0;

	for (const auto& vertex : interleaved_attribs)
	{
		stride += vertex.repeat * vertex.num_of_type * vertex.type_size;
	}

	unsigned init_offset = offset;

	for (const auto& vertex : interleaved_attribs)
	{
		for (unsigned int i = 0; i < vertex.repeat; ++i)
		{
			bind(vertex.index + i, vertex.num_of_type, vertex.type, vertex.normalized, stride, init_offset, divisor);
			init_offset += vertex.num_of_type * vertex.type_size;
		}
	}

	return *this;
}

const std::vector<BufferVertexAttribDefinition> &VaoLayout::get_definitions() const
{
	return definitions;
}

const std::vector<BufferUse> &VaoLayout::get_used_buffers() const
{
	return used_buffers;
}

bool VaoLayout::operator< (const VaoLayout &other) const
{
	return (used_buffers < other.used_buffers) || (definitions < other.definitions);
}

} // namespace Core
} // namespace GM
