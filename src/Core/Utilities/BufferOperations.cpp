#include "GM/Core/Utilities/BufferOperations.h"

namespace GM {
namespace Core {

void BufferOperations::upload_unsafe(const BufferObjectPtr &buffer,
	std::function<void(void *destination, size_t size)> upload_function,
	GLintptr offset, GLsizeiptr size)
{
	GLsizeiptr length = size;
	if (length == 0)
	{
		length = buffer->get_size();
	}
	else if (size > buffer->get_size())
	{
		throw std::runtime_error("Upload size is bigger than buffer size!");
	}
	else if (offset + size > buffer->get_size())
	{
		throw std::runtime_error("Upload size and offset is bigger than buffer's total size");
	}

	buffer->bind(); // Or maybe make a templated version for a custom buffer type?
	// Or may be assume the buffer is bound?

	void *destination = glMapBufferRange(buffer->get_primary_type(), offset, length,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

	if (destination == nullptr)
	{
		throw std::runtime_error("Unable to map buffer range!");
	}


	upload_function(destination, length);

	glUnmapBuffer(buffer->get_primary_type());
}

void BufferOperations::upload_unsafe(const BufferAllocation &buffer_allocation,
		std::function<void(void *destination, size_t size)> upload_function)
{
	upload_unsafe(buffer_allocation.buffer, upload_function,
		buffer_allocation.offset, buffer_allocation.allocated_size);
}

} // namespace Core
} // namespace GM
