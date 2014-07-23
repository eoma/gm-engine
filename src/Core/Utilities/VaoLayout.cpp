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
		throw std::runtime_error("Can not bind to buffer 0");
	}

	active_buffer = buffer;
	active_type = 0;

	return *this;
}

VaoLayout &VaoLayout::use_as(const unsigned int buffer_type) {
	if (active_buffer == nullptr || buffer_type == 0)
	{
		throw std::runtime_error("Can't use a 0-buffer or a buffer bound as nothing");
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
	const unsigned int size_per_index,
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
		throw std::runtime_error("Can only bind vertex attributes if buffer is GL_ARRAY_BUFFER!");
	}

	BufferVertexAttribDefinition def;

	def.buffer = active_buffer;
	def.index = index;
	def.size_per_index = size_per_index;
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
