#pragma once

#include "../GL/Texture.h"
#include <memory>

namespace GM {
namespace Core {

class ReadWriteTexture; typedef std::shared_ptr<ReadWriteTexture> ReadWriteTexturePtr;

/**
 * Enables convenient swapping of textures. Very useful in render passes.
 * This texture can be registered in TextureManager.
 *
 * Every writable texture can be bound as a normal texture
 */
class ReadWriteTexture : public Texture
{
public:
	ReadWriteTexture(unsigned int type);
	virtual ~ReadWriteTexture();

	const TexturePtr &get_readable() const { return readable; }
	const TexturePtr &get_writable() const { return writable; }

	void set_readable(const TexturePtr &new_readable);
	void set_writable(const TexturePtr &new_writable);

	// readable becomes writable and writable becomes readable.
	void flip();

protected:
	TexturePtr readable;
	TexturePtr writable;
};

} // namespace Core
} // namespace GM
