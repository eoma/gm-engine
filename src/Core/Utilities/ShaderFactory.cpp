#include "GM/Core/Utilities/ShaderFactory.h"

#include <ClanLib/core.h>

#include <GL/gl3w.h>

#include <iostream>

namespace GM {
namespace Core {

// FIXME: Switch to using methods defined in OpenGL 4.3 (extension ARB_program_interface_query)
// when it is added to Mesa

ShaderPtr ShaderFactory::make_program(const std::vector<ShaderSource> &shader_sources)
{
	ShaderPtr shader = std::make_shared<Shader>();

	std::vector<unsigned int> components;
	components.reserve(shader_sources.size());

	for (const ShaderSource &shader_source : shader_sources)
	{
		unsigned int component = compile_shader(shader_source);
		if (component > 0)
		{
			components.push_back(component);
			glAttachShader(shader->get_handle(), component);
		}
	}

	glLinkProgram(shader->get_handle());

	int linked = GL_FALSE, info_log_length = 0;
	glGetProgramiv(shader->get_handle(), GL_LINK_STATUS, &linked);
	glGetProgramiv(shader->get_handle(), GL_INFO_LOG_LENGTH, &info_log_length);

	if(info_log_length > 1)
	{
		std::vector<char> buffer(info_log_length);
		glGetProgramInfoLog(shader->get_handle(), info_log_length, NULL, buffer.data());

		std::cerr << std::string(buffer.begin(), buffer.end());

		// throw exception?
	}

	if(linked == GL_FALSE)
		throw clan::Exception("Failed to compile shader program!");

	for (auto component : components)
	{
		glDeleteShader(component);
	}

	shader->set_uniform_infos(get_uniform_infos(shader->get_handle()));
	shader->set_uniform_block_infos(get_uniform_block_infos(shader->get_handle()));
	shader->set_attribute_infos(get_attribute_infos(shader->get_handle()));

	return shader;
}

unsigned int ShaderFactory::compile_shader(const ShaderSource &shader_source)
{
	unsigned int shader_component = glCreateShader(shader_source.shader_type);

	const char * content_ptr = shader_source.content.c_str();
	glShaderSource(shader_component, 1, &content_ptr, NULL);
	glCompileShader(shader_component);

	//std::cout << "Compiles shader content..." << std::endl << content_ptr << std::endl;

	GLint result = GL_FALSE;
	glGetShaderiv(shader_component, GL_COMPILE_STATUS, &result);

	int info_log_length = 0;
	glGetShaderiv(shader_component, GL_INFO_LOG_LENGTH, &info_log_length);
	if(info_log_length > 1)
	{
		std::vector<char> buffer(info_log_length);
		glGetShaderInfoLog(shader_component, info_log_length, NULL, buffer.data());

		std::cerr << shader_source.name << ": " << std::string(buffer.begin(), buffer.end()) << std::endl;

		// throw exception?
	}

	if (result == GL_FALSE)
	{
		glDeleteShader(shader_component);
		shader_component = 0;
	}

	return shader_component;
}

std::vector<ShaderVariableInfo> ShaderFactory::get_attribute_infos(const unsigned int program)
{
	std::vector<ShaderVariableInfo> attribute_infos;
	std::vector<char> name_buffer;

	int active_attributes = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &active_attributes);
	attribute_infos.reserve(active_attributes);
	
	int max_attribute_name_length = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attribute_name_length);
	name_buffer.assign(max_attribute_name_length+1, '\0');

	for (int i = 0; i < active_attributes; ++i)
	{
		int name_length = 0;
		int size = 0;
		GLenum type = 0;
		
		glGetActiveAttrib(program, i, name_buffer.size(), &name_length, &size, &type, name_buffer.data());

		std::string name(name_buffer.begin(), name_buffer.begin() + name_length);

		// Get location
		int location = glGetAttribLocation(program, name.c_str());

		attribute_infos.emplace_back(name, type, size, location);
	}

	return attribute_infos;
}

std::vector<ShaderVariableInfo> ShaderFactory::get_uniform_infos(const unsigned int program, const int uniform_block_index)
{
	std::vector<ShaderVariableInfo> uniform_infos;
	std::vector<char> name_buffer;

	unsigned int active_uniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, (int*)&active_uniforms);
	uniform_infos.reserve(active_uniforms);
	
	unsigned int max_uniform_name_length = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, (int*)&max_uniform_name_length);
	name_buffer.assign(max_uniform_name_length+1, '\0');

	for (unsigned int i = 0; i < active_uniforms; ++i)
	{
		int name_length = 0;
		int size = 0;
		GLenum type = 0;

		// We will not process the current uniform index if it is not 
		// equal to the current uniform index (default is -1)
		int current_uniform_block_index = 0;
		glGetActiveUniformsiv(program, 1, &i, GL_UNIFORM_BLOCK_INDEX, &current_uniform_block_index);

		if (current_uniform_block_index != uniform_block_index)
		{
			continue;
		}

		// Get uniform name	
		glGetActiveUniformName(program, i, name_buffer.size(), &name_length, name_buffer.data());
		std::string name(name_buffer.begin(), name_buffer.begin() + name_length);

		// Get uniform type
		glGetActiveUniformsiv(program, 1, &i, GL_UNIFORM_TYPE, (GLint*)&type);

		// Get uniform type
		glGetActiveUniformsiv(program, 1, &i, GL_UNIFORM_SIZE, &size);

		// Get uniform location;
		int location = glGetUniformLocation(program, name.c_str());

		// Get uniform offset
		int offset = -1;
		glGetActiveUniformsiv(program, 1, &i, GL_UNIFORM_OFFSET, &offset);

		// Get array stride
		int array_stride = -1;
		glGetActiveUniformsiv(program, 1, &i, GL_UNIFORM_ARRAY_STRIDE, &array_stride);

		// Get matrix stride
		int matrix_stride = -1;
		glGetActiveUniformsiv(program, 1, &i, GL_UNIFORM_MATRIX_STRIDE, &matrix_stride);

		uniform_infos.emplace_back(name, type, size, location, offset, array_stride, matrix_stride);
	}

	return uniform_infos;
}

std::vector<UniformBlockInfo> ShaderFactory::get_uniform_block_infos(const unsigned int program)
{
	std::vector<UniformBlockInfo> uniform_block_infos;
	std::vector<char> name_buffer;

	int active_uniform_blocks = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &active_uniform_blocks);
	
	int max_uniform_block_name_length = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_uniform_block_name_length);
	name_buffer.assign(max_uniform_block_name_length+1, '\0');

	for (int i = 0; i < active_uniform_blocks; ++i)
	{
		int name_length = 0;
		int size = 0;
		int active_uniforms = 0;
		
		// Get name of block		
		glGetActiveUniformBlockName(program, i, name_buffer.size(), &name_length, name_buffer.data());
		std::string name(name_buffer.begin(), name_buffer.begin() + name_length);

		// Get minimum total buffer size (implementation dependent)
		glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

		// Get number of active uniforms in block
		glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniforms);

		uniform_block_infos.emplace_back(name, i, size, get_uniform_infos(program, i));
	}

	return uniform_block_infos;
}

} // namespace Core
} // namespace GM
