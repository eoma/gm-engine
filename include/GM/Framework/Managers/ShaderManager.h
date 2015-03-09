#pragma once

#include "../../Core/Utilities/ShaderSource.h"

#include "ClanLib/core.h"

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace GM {

namespace Core {
class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
}

namespace Framework {

class ShaderTemplateManager; typedef std::shared_ptr<ShaderTemplateManager> ShaderTemplateManagerPtr;
typedef unsigned int ShaderId;

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();

	void set_glsl_path(const std::string &glsl_path) { this->glsl_path = glsl_path; }

	bool contains(const std::string& name) const;

	// Great for custom made shaders
	void add(const std::string &name, const Core::ShaderPtr &shader);

	Core::ShaderPtr get(const std::string &name) const;

	Core::ShaderPtr get_or_create(const std::string &name);
	Core::ShaderPtr get_or_create(const std::string &name, const std::string &vs_file, const std::string &fs_file, bool rasterizer_discard);
	Core::ShaderPtr get_or_create(const std::string &name, const std::string &vs_file, const std::string &gs_file, const std::string &fs_file, bool rasterizer_discard);
	Core::ShaderPtr get_or_create(
		const std::string &name, const std::string &vs_file, const std::string &gs_file, const std::string &fs_file, 
		const std::string &tess_ctrl_file, const std::string &tess_eval_file, const std::string &compute_file, bool rasterizer_discard);

	void add_templates(const std::string &template_filename);

	/**
	 * The on_shader_added() method returns a signal that you can register to for
	 * when you want to listen when shaders have been added to this manager.
	 *
	 * This is useful for objects that rely on newly added shaders,
	 * eg. the uniform buffer block manager that scan the shader for uniform buffer blocks.
	 */
	clan::Signal<void(const std::string &name, const Core::ShaderPtr &shader)> &on_shader_added();

private:

	// Determine if file is cached, if it is return the cached content,
	// if not load, cache and return the file content.
	const std::string &load_contents(const std::string &file_name);

private:
	clan::Signal<void(const std::string &name, const Core::ShaderPtr &shader)> shader_added;

	std::unordered_map<std::string, std::string> file_content_cache;

	// Useful to map what files a specific shader is using.
	std::unordered_map<Core::ShaderPtr, std::set<std::string>> shader_to_file_deps;

	// Useful for mapping which shader pograms are using a file.
	std::unordered_map<std::string, std::set<Core::ShaderPtr>> file_to_shader_deps;

	//Associates a name with a shader.
	std::unordered_map<std::string, Core::ShaderPtr> name_to_shader;
	std::unordered_map<Core::ShaderPtr, std::string> shader_to_name;

	std::string glsl_path;
	ShaderTemplateManagerPtr template_manager;
};

typedef std::shared_ptr<ShaderManager> ShaderManagerPtr;

} // namespace Framework
} // namespace GM
