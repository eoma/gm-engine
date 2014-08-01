#include "GM/Core/GL/Shader.h"

#include "GL/gl3w.h"

#include <algorithm>

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

void Shader::set_attribute_infos(const std::vector<ShaderVariableInfo> &new_attribute_infos)
{
	attribute_infos = new_attribute_infos;
}

void Shader::set_uniform_infos(const std::vector<ShaderVariableInfo> &new_uniform_infos)
{
	uniform_infos = new_uniform_infos;
}

bool Shader::contains_uniform(const std::string &name) const
{
	auto iter = std::find_if(uniform_infos.begin(), uniform_infos.end(),
		[&name](const ShaderVariableInfo &info)
		{
			return info.name == name;
		});

	return (iter != uniform_infos.end());
}

ShaderVariableInfo Shader::get_uniform_info(const std::string &name) const
{
	ShaderVariableInfo info;

	std::find_if(uniform_infos.begin(), uniform_infos.end(),
		[&info, &name](const ShaderVariableInfo &search_info) mutable
		{
			if (search_info.name == name) {
				info = search_info;
				return true;
			};

			return false;
		});

	return info;
}

} // namespace Core
} // namespace GM
