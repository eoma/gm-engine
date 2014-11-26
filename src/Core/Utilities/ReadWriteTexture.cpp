#include "GM/Core/Utilities/ReadWriteTexture.h"

#include "GL/gl3w.h"
#include <ClanLib/core.h>

namespace GM {
namespace Core {

ReadWriteTexture::ReadWriteTexture(unsigned int type)
: Texture(type, false)
{
}

ReadWriteTexture::~ReadWriteTexture()
{
	// We do not own the handle, reset it before parent destructor runs
	handle = 0;
}

void ReadWriteTexture::set_readable(const TexturePtr &new_readable)
{
	if (type != new_readable->get_type())
	{
		throw clan::Exception("Readable texture must have same type as ReadWriteTexture!");
	}

	readable = new_readable;
}

void ReadWriteTexture::set_writable(const TexturePtr &new_writable)
{
	if (type != new_writable->get_type())
	{
		throw clan::Exception("Writable texture must have same type as ReadWriteTexture!");
	}

	writable = new_writable;
	handle = writable->get_handle();
}

void ReadWriteTexture::flip()
{
	std::swap(readable, writable);
	handle = writable->get_handle();
}

} // namespace Core
} // namespace GM
