#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

namespace GM {

namespace Core {

struct ShaderSource {
	// This struct passed on to the subsystem resposnsible for compiling shaders
	std::string name; // For example file name
	std::string content; // The shader content
	unsigned int shader_type; // The shader type, usually one of GL_{VERTEX,TESS_CONTROL,TESS_EVALUATION,GEOMETRY,FRAGMENT,COMPUTE}_SHADER

	ShaderSource(const std::string &name, const std::string &content, const unsigned int shader_type)
	: name(name)
	, content(content)
	, shader_type(shader_type)
	{
	}
};


} // namespace Core

namespace Framework {

typedef unsigned int ShaderId;

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();

	ShaderId load(const std::string &vs_file, const std::string &gs_file, const std::string &fs_file);

private:

	// Determine if file is cached, if it is return the cached content,
	// if not load, cache and return the file content.
	const std::string &load_contents(const std::string &file_name);

private:
	std::unordered_map<std::string, std::string> file_content_cache;

	// Useful to map what files a specific shader is using.
	std::unordered_map<ShaderId, std::set<std::string>> shader_to_file_deps;

	// Useful for mapping which shader pograms are using a file.
	std::unordered_map<std::string, std::set<ShaderId>> file_to_shader_deps;
};

} // namespace Framework
} // namespace GM
