#include <GM/Framework/Templates/MaterialTemplateManager.h>
#include <GM/Framework/Templates/MaterialTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>

#include <GM/Framework/Utilities/Material.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

MaterialTemplateManager::MaterialTemplateManager() {

}

void MaterialTemplateManager::get(const std::string &template_name, std::function<MaterialPtr(const MaterialTemplateManager::Template &)> func) {
	if (func == nullptr)
		return;

	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_requirement((*it_require), (*it_template));
			}

			auto material = func((*it_template));

			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_properties((*it_require), material);
			}

			apply_properties(it_template->name, material);
			

			return;
		}
	}
}

void MaterialTemplateManager::apply_requirement(const std::string &template_name, MaterialTemplateManager::Template &t) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			if (t.shader.empty()) t.shader = it_template->shader;
			return;
		}
	}
}

void MaterialTemplateManager::apply_properties(const std::string &template_name, const MaterialPtr &material) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_property = it_template->properties.begin(); it_property != it_template->properties.end(); ++it_property)
			{
				PropertySerializer::create_and_add_property(*material, (*it_property).type_id, (*it_property).name, (*it_property).value);
			}
			return;
		}
	}
}

void MaterialTemplateManager::add_templates(const std::string &template_filename) {
	try
	{
		MaterialTemplateParser::parse_templates(
			TextIO::load_contents(template_filename),
			[this](const MaterialTemplateManager::Template &t)
		{
			templates.push_back(t);
		});
	}
	catch (Exception e)
	{
		throw Exception(string_format("%1: %2", template_filename, e.what()));
	}
}
