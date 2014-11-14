#include <GM/Framework/Templates/ShaderTemplateManager.h>
#include <GM/Framework/Templates/ShaderTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>

#include <GM/Core/GL/Shader.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

ShaderTemplateManager::ShaderTemplateManager() {

}

void ShaderTemplateManager::get(const std::string &template_name, std::function<void(const ShaderTemplateManager::Template &)> func) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_requirement((*it_require), (*it_template));
			}
			if(func) func((*it_template));

			return;
		}
	}
}

void ShaderTemplateManager::apply_requirement(const std::string &template_name, ShaderTemplateManager::Template &t) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			if (t.vertex_shader.empty()) t.vertex_shader = it_template->vertex_shader;
			if (t.fragment_shader.empty()) t.fragment_shader = it_template->fragment_shader;
			if (t.geometry_shader.empty()) t.geometry_shader = it_template->geometry_shader;
			if (t.tesselation_control_shader.empty()) t.tesselation_control_shader = it_template->tesselation_control_shader;
			if (t.tesselation_evaluation_shader.empty()) t.tesselation_evaluation_shader = it_template->tesselation_evaluation_shader;
			if (t.compute_shader.empty()) t.compute_shader = it_template->compute_shader;
			// FIXME: Should we be setting rasterizer discard here too?
			return;
		}
	}
}

void ShaderTemplateManager::add_templates(const std::string &template_filename) {
	try
	{
		ShaderTemplateParser::parse_templates(
			TextIO::load_contents(template_filename),
			[this](const ShaderTemplateManager::Template &t)
		{
			templates.push_back(t);
		});
	}
	catch (Exception &e)
	{
		throw Exception(string_format("%1: %2", template_filename, e.what()));
	}
}
