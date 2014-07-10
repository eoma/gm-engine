#pragma once

#include <string>

namespace GM {
namespace Core {

struct ShaderSource {
	// This struct passed on to the subsystem resposnsible for compiling shaders
	std::string name; // For example file name
	std::string content; // The shader content
	unsigned int shader_type; // The shader type, usually one of GL_{VERTEX,TESS_CONTROL,TESS_EVALUATION,GEOMETRY,FRAGMENT,COMPUTE}_SHADER

	ShaderSource(const std::string &name, const std::string &content, const unsigned int shader_type);
};

} // namespace Core
} // namespace GM
