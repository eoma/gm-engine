#include "GM/Core/GL/Shader.h"

#include "GL/gl3w.h"

namespace GM {
namespace Core {

Shader::Shader()
{
	handle = glCreateProgram();
}

Shader::~Shader()
{
	glDeleteProgram(handle);
}

void Shader::bind() const
{
	glUseProgram(handle);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::set_attribute_info(const std::vector<ShaderVariableInfo> &new_attribute_info)
{
	attribute_info = new_attribute_info;
}

void Shader::set_uniform_info(const std::vector<ShaderVariableInfo> &new_uniform_info)
{
	uniform_info = new_uniform_info;
}

} // namespace Core
} // namespace GM
