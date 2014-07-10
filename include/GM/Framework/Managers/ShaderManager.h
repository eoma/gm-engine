#pragma once

#include "../../Core/Utilities/ShaderSource.h"

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace GM {
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
