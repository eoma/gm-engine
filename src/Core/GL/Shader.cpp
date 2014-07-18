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

} // namespace Core
} // namespace GM
