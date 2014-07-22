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

	static std::vector<ShaderUniformInfo> get_uniform_info(const unsigned int program);
};

} // namespace Core
} // namespace GM
