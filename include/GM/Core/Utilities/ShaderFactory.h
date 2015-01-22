#pragma once

#include "../GL/Shader.h"
#include "ShaderSource.h"

#include <string>
#include <vector>

namespace GM {
namespace Core {

class ShaderFactory
{
public:
	static ShaderPtr make_program(const std::vector<ShaderSource> &shader_sources);

private:
	static unsigned int compile_shader(const ShaderSource &shader_source);

	// These may be generalized using glGetProgramResource
	static std::vector<ShaderVariableInfo> get_attribute_infos(const unsigned int program);
	static std::vector<ShaderVariableInfo> get_uniform_infos(const unsigned int program, const int uniform_block_index = -1);
	static std::vector<UniformBlockInfo> get_uniform_block_infos(const unsigned int program);
};

} // namespace Core
} // namespace GM
