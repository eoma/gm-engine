#include "GM/Core/GL/BufferObject.h"

namespace GM {
namespace Core {

// FIXME: When GL comes...

BufferObject::BufferObject(unsigned int primary_type)
: primary_type(primary_type)
, current_type(0)
, handle(0)
{
	//glGenBuffers(1, &handle);
	bind();
}

BufferObject::~BufferObject()
{
	unbind();
	//glDeleteBuffers(1, &handle);
}

void BufferObject::bind()
{
	bind_as(primary_type);
}

void BufferObject::bind_as(const unsigned int type)
{
	current_type = type;
	// glBindBuffer(type, handle);
}

void BufferObject::unbind()
{
	if (current_type != 0) {
		//glBindBuffer(current_type, 0);
		current_type = 0;
	}
}

} // namespace Core
} // namespace GM
