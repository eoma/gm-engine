#include <GM/Framework/Templates/TextureFormatTemplateManager.h>
#include <GM/Framework/Templates/TextureFormatTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>

#include <GM/Core/GL/Shader.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

TextureFormatTemplateManager::TextureFormatTemplateManager() {

}

void TextureFormatTemplateManager::get(const std::string &template_name, std::function<void(const TextureFormatTemplateManager::Template &)> func) {
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

void TextureFormatTemplateManager::apply_requirement(const std::string &template_name, TextureFormatTemplateManager::Template &t) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			if (t.min_filter.empty()) t.min_filter = it_template->min_filter;
			if (t.mag_filter.empty()) t.mag_filter = it_template->mag_filter;
			if (t.wrap_s.empty()) t.wrap_s = it_template->wrap_s;
			if (t.wrap_t.empty()) t.wrap_t = it_template->wrap_t;
			// FIXME: Should we be setting generate mipmap here too?
			return;
		}
	}
}

void TextureFormatTemplateManager::add_templates(const std::string &template_filename) {
	TextureFormatTemplateParser::parse_templates(
		TextIO::load_contents(template_filename),
		[this](const TextureFormatTemplateManager::Template &t)
	{
		templates.push_back(t);
	});
}
