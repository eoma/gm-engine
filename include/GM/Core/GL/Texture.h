#pragma once

#include <memory>

namespace GM {
namespace Core {

class Texture; typedef std::shared_ptr<Texture> TexturePtr;

class Texture
{
public:
	Texture(unsigned int type);
	virtual ~Texture();

	void bind() const;
	void unbind() const;

	unsigned int get_type() const { return type; }
	unsigned int get_handle() const { return handle; }

protected:
	Texture(unsigned int type, bool create_handle);

	unsigned int type;
	unsigned int handle;
};

} // namespace Core
} // namespace GM
