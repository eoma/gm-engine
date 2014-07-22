#pragma once

#include <GL/gl3w.h>

#include <memory>
#include <vector>

namespace GM {
namespace Core {

class Shader; typedef std::shared_ptr<Shader> ShaderPtr;

struct ShaderUniformInfo
{
	std::string name;
	GLenum type;
	unsigned int size; // How many multiples of type are there? Relevant for array
	int location; // be default -1

	ShaderUniformInfo(const std::string &name, const GLenum type, const unsigned int size, const int location = -1) : name(name), type(type), size(size), location(location) {};
};

class Shader
{
public:
	Shader();
	~Shader();

	void bind() const;
	void unbind() const;

	unsigned int get_handle() const { return handle; }

	const std::vector<ShaderUniformInfo> &get_uniform_info() const { return uniform_info; }
	void set_uniform_info(const std::vector<ShaderUniformInfo> &new_uniform_info);

private:
	unsigned int handle;

	std::vector<ShaderUniformInfo> uniform_info;
};

} // namespace Core
} // namespace GM
