#include "GM/Core/Utilities/ShaderFactory.h"

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
		throw std::runtime_error("Failed to compile shader program!");

	for (auto component : components)
	{
		glDeleteShader(component);
	}

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

} // namespace Core
} // namespace GM
