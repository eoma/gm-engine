#include "GM/Core/Utilities/ShaderFactory.h"

#include <ClanLib/core.h>

#include <GL/gl3w.h>

#include <iostream>

namespace GM {
namespace Core {

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

		//if (name.compare(0, 3, "gl_") != 0)
		{
			// We do not add uniform values that can not be modified by glUniform* og glProgramUniform*

			attribute_infos.emplace_back(name, type, size, glGetAttribLocation(program, name.c_str()));
		}
	}

	return attribute_infos;
}

std::vector<ShaderVariableInfo> ShaderFactory::get_uniform_infos(const unsigned int program)
{
	std::vector<ShaderVariableInfo> uniform_infos;
	std::vector<char> name_buffer;

	int active_uniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &active_uniforms);
	uniform_infos.reserve(active_uniforms);
	
	int max_uniform_name_length = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);
	name_buffer.assign(max_uniform_name_length+1, '\0');

	for (int i = 0; i < active_uniforms; ++i)
	{
		int name_length = 0;
		int size = 0;
		GLenum type = 0;
		
		glGetActiveUniform(program, i, name_buffer.size(), &name_length, &size, &type, name_buffer.data());

		std::string name(name_buffer.begin(), name_buffer.begin() + name_length);

		//if (name.compare(0, 3, "gl_") != 0)
		{
			// We do not add uniform values that can not be modified by glUniform* og glProgramUniform*

			uniform_infos.emplace_back(name, type, size, glGetUniformLocation(program, name.c_str()));
		}
	}

	return uniform_infos;
}


} // namespace Core
} // namespace GM
