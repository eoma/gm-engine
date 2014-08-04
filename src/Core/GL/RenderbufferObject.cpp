#include "GM/Core/GL/RenderbufferObject.h"

namespace GM {
namespace Core {

RenderbufferObject::RenderbufferObject(unsigned int type, unsigned int w, unsigned int h)
	: handle(0), type(type), w(w), h(h)
{
	glGenRenderbuffers(1, &handle);
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, type, w, h);
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
