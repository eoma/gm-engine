#pragma once

#include <memory>

namespace GM {
namespace Core {

class Texture; typedef std::shared_ptr<Texture> TexturePtr;

class Texture
{
public:
	Texture(unsigned int type);
	~Texture();

	void bind() const;
	void unbind() const;

	unsigned int get_type() const { return type; }
	unsigned int get_handle() const { return handle; }

private:
	unsigned int type;
	unsigned int handle;
};

} // namespace Core
} // namespace GM
