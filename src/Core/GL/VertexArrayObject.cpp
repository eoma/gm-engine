#include "GM/Core/GL/VertexArrayObject.h"

#include "GL/gl3w.h"

namespace GM {
namespace Core {

VertexArrayObject::VertexArrayObject()
: handle(0)
{
	glGenVertexArrays(1, &handle);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &handle);
}

void VertexArrayObject::bind() const
{
	glBindVertexArray(handle);
}

void VertexArrayObject::unbind() const
{
	glBindVertexArray(0);
}

} // namespace Core
} // namespace GM
