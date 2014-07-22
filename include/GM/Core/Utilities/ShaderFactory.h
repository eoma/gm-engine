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
};

} // namespace Core
} // namespace GM
