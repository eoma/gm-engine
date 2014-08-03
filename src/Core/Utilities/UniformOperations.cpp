#include "GM/Core/Utilities/UniformOperations.h"

#include <glm/ext.hpp>

#include <GL/gl3w.h>

namespace GM {
namespace Core {

//
// Primitives
//
void update_uniform(unsigned int program, int location, char data)
{
	glProgramUniform1i(program, location, data);
}

void update_uniform(unsigned int program, int location, unsigned char data)
{
	glProgramUniform1ui(program, location, data);
}

void update_uniform(unsigned int program, int location, short data)
{
	glProgramUniform1i(program, location, data);
}

void update_uniform(unsigned int program, int location, unsigned short data)
{
	glProgramUniform1ui(program, location, data);
}

void update_uniform(unsigned int program, int location, int data)
{
	glProgramUniform1i(program, location, data);
}

void update_uniform(unsigned int program, int location, unsigned int data)
{
	glProgramUniform1ui(program, location, data);
}

void update_uniform(unsigned int program, int location, float data)
{
	glProgramUniform1f(program, location, data);
}

//
// Vectors
//
void update_uniform(unsigned int program, int location, const glm::vec2 &data)
{
	glProgramUniform2fv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::ivec2 &data)
{
	glProgramUniform2iv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::uvec2 &data)
{
	glProgramUniform2uiv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::vec3 &data)
{
	glProgramUniform3fv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::ivec3 &data)
{
	glProgramUniform3iv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::uvec3 &data)
{
	glProgramUniform3uiv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::vec4 &data)
{
	glProgramUniform4fv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::ivec4 &data)
{
	glProgramUniform4iv(program, location, 1, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::uvec4 &data)
{
	glProgramUniform4uiv(program, location, 1,  glm::value_ptr(data));
}

//
// Matrices
//

void update_uniform(unsigned int program, int location, const glm::mat2 &data)
{
	glProgramUniformMatrix2fv(program, location, 1, false, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::mat3 &data)
{
	glProgramUniformMatrix3fv(program, location, 1, false, glm::value_ptr(data));
}

void update_uniform(unsigned int program, int location, const glm::mat4 &data)
{
	glProgramUniformMatrix4fv(program, location, 1, false, glm::value_ptr(data));
}

//
// Textures
//

void update_uniform(unsigned int program, int location, const Core::TexturePtr &data)
{
	auto handle = (GLint)data->get_handle();
	glProgramUniform1i(program, location, 0);
}

} // namespace Core
} // namespace GM
