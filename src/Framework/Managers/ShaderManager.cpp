#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/IO/ShaderIO.h"

#include <ClanLib/core.h>

#include "GM/Framework/Utilities/Tools.h"

#include <algorithm>

namespace GM {
namespace Framework {

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

ShaderId ShaderManager::load(const std::string &vs_file, const std::string &fs_file)
{
	return load(vs_file, "", fs_file);
}

ShaderId ShaderManager::load(const std::string &vs_file, const std::string &gs_file, const std::string &fs_file)
{
	ShaderId id = 0;

	std::vector<std::set<ShaderId>> shader_id_sets;

	if (!vs_file.empty()) shader_id_sets.push_back(file_to_shader_deps[vs_file]);
	if (!gs_file.empty()) shader_id_sets.push_back(file_to_shader_deps[gs_file]);
	if (!fs_file.empty()) shader_id_sets.push_back(file_to_shader_deps[fs_file]);

	std::set<ShaderId> singular_shader = general_set_intersection(shader_id_sets);

	if (!singular_shader.empty()) // TODO: How to determine if it is indeed unique
	{
		id = *(singular_shader.begin());
	}

	if (id == 0) // found no preexisting shader
	{
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

		// TODO: What about errors?
		// TODO: Actual implementation of ShaderFactory
		id = 1 /*Core::ShaderFactory::make_program(shader_sources)*/;

		for (const Core::ShaderSource &res : shader_sources) {
			shader_to_file_deps[id].insert(res.name);
			file_to_shader_deps[res.name].insert(id);
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
			ShaderIO::load_contents(file_name)
		)).first;
	}
	
	return iter->second;
}

} // namespace Framework
} // namespace GM
