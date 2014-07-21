#include <GM/Framework/Templates/TextureTemplateManager.h>
#include <GM/Framework/Templates/TextureTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>

#include <GM/Core/GL/Shader.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

TextureTemplateManager::TextureTemplateManager() {

}

void TextureTemplateManager::get(const std::string &template_name, std::function<void(const TextureTemplateManager::Template &)> func) {
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

void TextureTemplateManager::apply_requirement(const std::string &template_name, TextureTemplateManager::Template &t) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			if (t.image.empty()) t.image = it_template->image;
			if (t.format.empty()) t.format = it_template->format;
			return;
		}
	}
}

void TextureTemplateManager::add_templates(const std::string &template_filename) {
	/*TextureTemplateParser::parse_templates(
		TextIO::load_contents(template_filename),
		[this](const TextureTemplateManager::Template &t)
	{
		templates.push_back(t);
	});*/
}
