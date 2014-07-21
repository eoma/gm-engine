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

void ShaderTemplateManager::apply(const std::string &template_name, const Core::ShaderPtr &shader) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_template((*it_require), shader);
			}
		}
	}
}

void ShaderTemplateManager::apply_template(const std::string &template_name, const Core::ShaderPtr &shader) {
	// TODO: Need a way to apply a template to a shader... not entirely sure, but this will probably require a rethinking at some level...
	// TODO: Maybe this just generates the "cache info" in ShaderManager, and thus it should take ShaderManager as input here instead of Shader?
}

void ShaderTemplateManager::add_templates(const std::string &template_filename) {
	ShaderTemplateParser::parse_templates(
		TextIO::load_contents(template_filename),
		[this](const ShaderTemplateManager::Template &t)
	{
		templates.push_back(t);
	});
}
