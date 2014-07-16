#pragma once

namespace GM {
namespace Core {

class Shader
{
public:
	Shader();
	~Shader();

	void bind() const;
	void unbind() const;

	unsigned int get_handle() const { return handle; };

private:
	unsigned int handle;
};

} // namespace Core
} // namespace GM
