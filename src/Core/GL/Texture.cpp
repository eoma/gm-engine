#include "GM/Core/GL/Texture.h"

#include "GL/gl3w.h"

namespace GM {
namespace Core {

Texture::Texture(unsigned int type)
: type(type)
, handle(0)
{
	glGenTextures(1, &handle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

void Texture::bind() const
{
	glBindTexture(type, handle);
}

void Texture::unbind() const
{
	glBindTexture(type, 0);
}

} // namespace Core
} // namespace GM
