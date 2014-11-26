#include "GM/Core/GL/Texture.h"

#include "GL/gl3w.h"

namespace GM {
namespace Core {

Texture::Texture(unsigned int type)
: Texture(type, true)
{
}

Texture::Texture(unsigned int type, bool create_handle)
: type(type)
, handle(0)
{
	if (create_handle) {
		glGenTextures(1, &handle);
	}
}

Texture::~Texture()
{
	if (handle != 0) {
		glDeleteTextures(1, &handle);
	}
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
