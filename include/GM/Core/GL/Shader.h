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
	int location; // default -1
	int offset; // default -1, only relevant for uniform blocks
	int matrix_stride; // default -1, only relevant for uniform blocks
	int array_stride; // default -1, only relevant for uniform blocks

	ShaderVariableInfo(const std::string &name = std::string(), const GLenum type = 0, const unsigned int size = 0, const int location = -1, const int offset = -1, const int matrix_stride = -1, const int array_stride = -1) : name(name), type(type), size(size), location(location), offset(offset), matrix_stride(matrix_stride), array_stride(array_stride) {};
};

struct UniformBlockInfo
{
	std::string name;
	int index;
	int size;
	std::vector<ShaderVariableInfo> active_uniforms;

	UniformBlockInfo(const std::string &name = std::string(), const int index = -1, int size = 0, const std::vector<ShaderVariableInfo> &active_uniforms = std::vector<ShaderVariableInfo>()) : name(name), index(index), size(size), active_uniforms(active_uniforms) {}
};

class Shader
{
public:
	Shader();
	~Shader();

	void bind() const;
	void unbind() const;

	unsigned int get_handle() const { return handle; }

	bool contains_uniform(const std::string &name) const;
	ShaderVariableInfo get_uniform_info(const std::string &name) const;
	const std::vector<ShaderVariableInfo> &get_uniform_infos() const { return uniform_infos; }

	const std::vector<ShaderVariableInfo> &get_attribute_infos() const { return attribute_infos; }

	const std::vector<UniformBlockInfo> &get_uniform_block_infos() const { return uniform_block_infos; }

	/**
	 * The following operations are used by the ShaderFactory
	 */
	void set_uniform_infos(const std::vector<ShaderVariableInfo> &new_uniforms);
	void set_uniform_block_infos(const std::vector<UniformBlockInfo> &new_uniform_blocks);
	void set_attribute_infos(const std::vector<ShaderVariableInfo> &new_attributes);

private:
	unsigned int handle;

	std::vector<ShaderVariableInfo> attribute_infos;
	std::vector<ShaderVariableInfo> uniform_infos;
	std::vector<UniformBlockInfo> uniform_block_infos;
};

} // namespace Core
} // namespace GM
