#pragma once

#include <memory>

namespace GM {
namespace Core {

class Shader; typedef std::shared_ptr<Shader> ShaderPtr;

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
