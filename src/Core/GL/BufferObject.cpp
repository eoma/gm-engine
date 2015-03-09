#include "GM/Core/GL/BufferObject.h"

namespace GM {
namespace Core {

BufferObject::BufferObject(unsigned int size, unsigned int primary_type, unsigned int usage)
: primary_type(primary_type)
, current_type(0)
, usage(usage)
, handle(0)
, size(size)
{
	glGenBuffers(1, &handle);
	bind();
	glBufferData(primary_type, size, nullptr, usage); // Initialize buffer. buffer will contain garbage
}

BufferObject::~BufferObject()
{
	unbind();
	glDeleteBuffers(1, &handle);
}

void BufferObject::bind()
{
	bind_as(primary_type);
}

void BufferObject::bind_as(const unsigned int type)
{
	current_type = type;
	glBindBuffer(type, handle);
}

void BufferObject::unbind()
{
	if (current_type != 0) {
		glBindBuffer(current_type, 0);
		current_type = 0;
	}
}

void BufferObject::upload(unsigned int size, unsigned int offset, const std::function<void(void *destination, size_t size)> &upload_function)
{
	BufferOperations::upload_unsafe(current_type, upload_function, size, offset);
}

void BufferObject::upload(const std::function<void(void *destination, size_t size)> &upload_function)
{
	upload(size, 0, upload_function);
}

} // namespace Core
} // namespace GM
