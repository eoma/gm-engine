#pragma once

#include <GL/gl3w.h>

#include <memory>
#include <vector>
#include <string>

namespace GM {
namespace Core {

class Shader; typedef std::shared_ptr<Shader> ShaderPtr;

struct ShaderVariableInfo
{
	std::string name;
	GLenum type;
	unsigned int size; // How many multiples of type are there? Relevant for array
	int location; // be default -1

	ShaderVariableInfo(const std::string &name = std::string(), const GLenum type = 0, const unsigned int size = 0, const int location = -1) : name(name), type(type), size(size), location(location) {};
};

class Shader
{
public:
	Shader();
	~Shader();

	void bind() const;
	void unbind() const;

	unsigned int get_handle() const { return handle; }

	const std::vector<ShaderVariableInfo> &get_uniform_infos() const { return uniform_infos; }
	void set_uniform_infos(const std::vector<ShaderVariableInfo> &new_uniform_info);
	bool contains_uniform(const std::string &name) const;
	ShaderVariableInfo get_uniform_info(const std::string &name) const;

	const std::vector<ShaderVariableInfo> &get_attribute_infos() const { return attribute_infos; }
	void set_attribute_infos(const std::vector<ShaderVariableInfo> &new_attribute_info);
private:
	unsigned int handle;

	std::vector<ShaderVariableInfo> attribute_infos;
	std::vector<ShaderVariableInfo> uniform_infos;
};

} // namespace Core
} // namespace GM
