#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/Templates/ShaderTemplateManager.h"
#include "GM/Framework/IO/TextIO.h"

#include <ClanLib/core.h>

#include "GM/Framework/Utilities/Tools.h"

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

ShaderId ShaderManager::get_or_create(const std::string &name)
{
	//First, test if the name has been cached.
	auto shader = get_or_create(name, "", "", "", "", "", "", false);
	if (shader) {
		return shader;
	}

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

	return shader;
}

ShaderId ShaderManager::get_or_create(const std::string &name, const std::string &vs_file, const std::string &fs_file, bool rasterizer_discard)
{
	return get_or_create(name, vs_file, "", fs_file, "", "", "", rasterizer_discard);
}

ShaderId ShaderManager::get_or_create(const std::string &name, const std::string &vs_file, const std::string &gs_file, const std::string &fs_file, bool rasterizer_discard)
{
	return get_or_create(name, vs_file, gs_file, fs_file, "", "", "", rasterizer_discard);
}

ShaderId ShaderManager::get_or_create(
	const std::string &name, const std::string &vs_file, const std::string &gs_file, const std::string &fs_file,
	const std::string &tess_ctrl_file, const std::string &tess_eval_file, const std::string &compute_file, bool rasterizer_discard)
{
	
	auto iter = name_to_shader.find(name);
	bool is_name_cached = iter != name_to_shader.end();

	ShaderId id = 0;

	std::vector<std::set<ShaderId>> shader_id_sets;

	if (!vs_file.empty()) shader_id_sets.push_back(file_to_shader_deps[vs_file]);
	if (!gs_file.empty()) shader_id_sets.push_back(file_to_shader_deps[gs_file]);
	if (!fs_file.empty()) shader_id_sets.push_back(file_to_shader_deps[fs_file]);
	// TODO: Expand to support tesselation and compute shaders

	// If no shader files were specified (only the name), then we have nothing more to do here other than returning
	// the result of the name to shader relation.
	if (shader_id_sets.empty())
	{
		if (is_name_cached) return iter->second;
		else return 0; // FIXME: Change to nullptr when ShaderId is replaced with Core::Shader
	}

	std::set<ShaderId> singular_shader = general_set_intersection(shader_id_sets);

	if (!singular_shader.empty()) // TODO: How to determine if it is indeed unique
	{
		id = *(singular_shader.begin());
	}

	if (id == 0) // found no preexisting shader
	{
		if (is_name_cached) {
			throw clan::Exception(clan::string_format("The name of the shader (%1) was cached, but its shader set did not match the one specified!", name));
		}

		std::vector<Core::ShaderSource> shader_sources;

		if (!vs_file.empty())
		{
			Core::ShaderSource res(vs_file, load_contents(vs_file), 0); //TODO: Replace with GL_VERTEX_SHADER
			shader_sources.push_back(res);
		}

		if (!gs_file.empty())
		{
			Core::ShaderSource res(gs_file, load_contents(gs_file), 1); //TODO: Replace with GL_GEOMETRY_SHADER
			shader_sources.push_back(res);
		}

		if (!fs_file.empty())
		{
			Core::ShaderSource res(fs_file, load_contents(fs_file), 2); //TODO: Replace with GL_FRAGMENT_SHADER
			shader_sources.push_back(res);
		}
		// TODO: Expand to support tesselation and compute shaders

		// TODO: What about errors?
		// TODO: What about rasterizer_discard?
		// TODO: Actual implementation of ShaderFactory
		id = 1 /*Core::ShaderFactory::make_program(shader_sources)*/;

		name_to_shader[name] = id;
		shader_to_name[id] = name;
		for (const Core::ShaderSource &res : shader_sources) {
			shader_to_file_deps[id].insert(res.name);
			file_to_shader_deps[res.name].insert(id);
			
		}
	}
	else
	{
		// Just to be safe, check that the name is what we expect
		if (is_name_cached) {
			if (iter->second != id) {
				auto name_of_id = shader_to_name[id];
				throw clan::Exception(clan::string_format("The name of the shader (%1) was cached, but its shader set did not match the one specified (%2)!", name, name_of_id));
			}
		}
		// If the name was not cached, but we found an id, something is wrong...
		else {
			auto name_of_id = shader_to_name[id];
			throw clan::Exception(clan::string_format("The name (%1) was not cached, but a shader is already associated with this combination of shader files (%2)!", name, name_of_id));
		}
	}

	return id;
}

const std::string &ShaderManager::load_contents(const std::string &file_name)
{
	auto iter = file_content_cache.find(file_name);

	if (iter == file_content_cache.end())
	{
		iter = file_content_cache.insert(std::make_pair(
			file_name, 
			TextIO::load_contents(file_name)
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
