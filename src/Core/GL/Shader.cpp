#include "GM/Core/GL/Shader.h"

namespace GM {
namespace Core {

// FIXME: When GL comes...

Shader::Shader()
{
	handle = /*glCreateProgram()*/ 0;
}

Shader::~Shader()
{
	//glDeleteProgram(handle);
}

void Shader::bind() const
{
	//glUseProgram(handle);
}

void Shader::unbind() const
{
	//glUseProgram(0);
}

} // namespace Core
} // namespace GM
