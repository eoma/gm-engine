#include "GM/Core/GL/RenderbufferObject.h"

namespace GM {
namespace Core {

RenderbufferObject::RenderbufferObject(unsigned int type, unsigned int width, unsigned int height)
	: handle(0), type(type), width(width), height(height)
{
	glGenRenderbuffers(1, &handle);
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
}

RenderbufferObject::~RenderbufferObject()
{
	glDeleteRenderbuffers(1, &handle);
}

void RenderbufferObject::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, handle);
}

void RenderbufferObject::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

} // namespace Core
} // namespace GM
