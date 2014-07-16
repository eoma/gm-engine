#pragma once

namespace GM {
namespace Core {

class Texture
{
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
