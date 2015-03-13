#include "GM/Framework/Managers/ShaderManager.h"

#include "GM/Core/GL/Shader.h"
#include "GM/Core/Utilities/ShaderFactory.h"

#include "GM/Framework/Templates/ShaderTemplateManager.h"
#include "GM/Framework/IO/TextIO.h"
#include "GM/Framework/Utilities/Tools.h"

#include <ClanLib/core.h>

#include <GL/gl3w.h>

#include <algorithm>

namespace GM {
namespace Framework {

ShaderManager::ShaderManager()
{
	template_manager = ShaderTemplateManagerPtr(new ShaderTemplateManager());
}

ShaderManager::~ShaderManager()
{
}

clan::Signal<void(const std::string &, const Core::ShaderPtr &)> &ShaderManager::on_shader_added()
{
	return shader_added;
}

bool ShaderManager::contains(const std::string &name) const
{
	return (name_to_shader.end() != name_to_shader.find(name));
}

void ShaderManager::add(const std::string &name, const Core::ShaderPtr &shader)
{
	if (!contains(name))
	{
		// maybe check that shader is not a nullptr?
		name_to_shader.insert(std::make_pair(name, shader));
		shader_to_name.insert(std::make_pair(shader, name));

		// Send out a signal that we've added a shader
		shader_added(name, shader);
	}
	else
	{
		// throw exception?
	}
}

Core::ShaderPtr ShaderManager::get(const std::string &name) const
{
	Core::ShaderPtr shader = nullptr;
	auto iter = name_to_shader.find(name);
	if (iter != name_to_shader.end())
	{
		shader = iter->second;
	}
	return shader;
}

Core::ShaderPtr ShaderManager::get_or_create(const std::string &name)
{
	// First, test if the name has been cached.
	auto shader = get(name);
	if (shader)
	{
		return shader;
	}

	//If not cached, let's see if there is a template description for this name.
	template_manager->get(name, [this, name, &shader](const ShaderTemplateManager::Template &t) {
		shader = get_or_create(name, 
			t.vertex_shader, 
			t.geometry_shader, 
			t.fragment_shader, 
			t.tesselation_control_shader, 
			t.tesselation_evaluation_shader, 
			t.compute_shader, 
			t.rasterizer_discard);
	});

	if (!shader)
	{
		throw clan::Exception(clan::string_format("Failed to get or create the shader %1.", name));
	}

	return shader;
}

Core::ShaderPtr ShaderManager::get_or_create(const std::string &name, const std::string &vs_file, const std::string &fs_file, bool rasterizer_discard)
{
	return get_or_create(name, vs_file, "", fs_file, "", "", "", rasterizer_discard);
}

Core::ShaderPtr ShaderManager::get_or_create(const std::string &name, const std::string &vs_file, const std::string &gs_file, const std::string &fs_file, bool rasterizer_discard)
{
	return get_or_create(name, vs_file, gs_file, fs_file, "", "", "", rasterizer_discard);
}

Core::ShaderPtr ShaderManager::get_or_create(
	const std::string &name, const std::string &vs_file, const std::string &gs_file, const std::string &fs_file,
	const std::string &tess_ctrl_file, const std::string &tess_eval_file, const std::string &compute_file, bool rasterizer_discard)
{
	
	auto iter = name_to_shader.find(name);
	bool is_name_cached = (iter != name_to_shader.end());

	Core::ShaderPtr shader = nullptr;

	std::vector<std::set<Core::ShaderPtr>> shader_sets;

	if (!vs_file.empty()) shader_sets.push_back(file_to_shader_deps[vs_file]);
	if (!gs_file.empty()) shader_sets.push_back(file_to_shader_deps[gs_file]);
	if (!fs_file.empty()) shader_sets.push_back(file_to_shader_deps[fs_file]);
	if (!tess_ctrl_file.empty()) shader_sets.push_back(file_to_shader_deps[tess_ctrl_file]);
	if (!tess_eval_file.empty()) shader_sets.push_back(file_to_shader_deps[tess_eval_file]);
	if (!compute_file.empty()) shader_sets.push_back(file_to_shader_deps[compute_file]);

	// If no shader files were specified (only the name), then we have nothing more to do here other than returning
	// the result of the name to shader relation.
	if (shader_sets.empty())
	{
		if (is_name_cached) return iter->second;
		else return nullptr;
	}

	std::set<Core::ShaderPtr> singular_shader = general_set_intersection(shader_sets);

	if (!singular_shader.empty()) // TODO: How to determine if it is indeed unique
	{
		shader = *(singular_shader.begin());
	}

	if (shader == nullptr) // found no preexisting shader
	{
		if (is_name_cached) {
			throw clan::Exception(clan::string_format("The name of the shader (%1) was cached, but its shader set did not match the one specified!", name));
		}

		std::vector<Core::ShaderSource> shader_sources;

		if (!vs_file.empty())
		{
			Core::ShaderSource res(vs_file, load_contents(vs_file), GL_VERTEX_SHADER);
			shader_sources.push_back(res);
		}

		if (!gs_file.empty())
		{
			Core::ShaderSource res(gs_file, load_contents(gs_file), GL_GEOMETRY_SHADER);
			shader_sources.push_back(res);
		}

		if (!fs_file.empty())
		{
			Core::ShaderSource res(fs_file, load_contents(fs_file), GL_FRAGMENT_SHADER);
			shader_sources.push_back(res);
		}

		if (!tess_ctrl_file.empty())
		{
			Core::ShaderSource res(tess_ctrl_file, load_contents(tess_ctrl_file), GL_TESS_CONTROL_SHADER);
			shader_sources.push_back(res);
		}

		if (!tess_eval_file.empty())
		{
			Core::ShaderSource res(tess_eval_file, load_contents(tess_eval_file), GL_TESS_EVALUATION_SHADER);
			shader_sources.push_back(res);
		}

		if (!compute_file.empty())
		{
			Core::ShaderSource res(compute_file, load_contents(compute_file), GL_COMPUTE_SHADER);
			shader_sources.push_back(res);
		}

		// TODO: What about errors?
		// TODO: What about rasterizer_discard?
		// TODO: Actual implementation of ShaderFactory
		shader = Core::ShaderFactory::make_program(shader_sources);

		add(name, shader);

		for (const Core::ShaderSource &res : shader_sources) {
			shader_to_file_deps[shader].insert(res.name);
			file_to_shader_deps[res.name].insert(shader);
		}


	}
	else
	{
		// Just to be safe, check that the name is what we expect
		if (is_name_cached) {
			if (iter->second != shader) {
				auto name_of_shader = shader_to_name[shader];
				throw clan::Exception(clan::string_format("The name of the shader (%1) was cached, but its shader set did not match the one specified (%2)!", name, name_of_shader));
			}
		}
		// If the name was not cached, but we found an id, something is wrong...
		else {
			auto name_of_shader = shader_to_name[shader];
			throw clan::Exception(clan::string_format("The name (%1) was not cached, but a shader is already associated with this combination of shader files (%2)!", name, name_of_shader));
		}
	}

	return shader;
}

const std::string &ShaderManager::load_contents(const std::string &file_name)
{
	auto iter = file_content_cache.find(file_name);

	if (iter == file_content_cache.end())
	{
		iter = file_content_cache.insert(std::make_pair(
			file_name, 
			TextIO::load_contents(glsl_path + "/" + file_name)
		)).first;
	}
	
	return iter->second;
}

void ShaderManager::add_templates(const std::string &template_filename)
{
	template_manager->add_templates(template_filename);
}

} // namespace Framework
} // namespace GM
