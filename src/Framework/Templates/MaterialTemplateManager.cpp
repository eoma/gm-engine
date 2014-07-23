#include <GM/Framework/Templates/MaterialTemplateManager.h>
#include <GM/Framework/Templates/MaterialTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>

#include <GM/Framework/Components/Material.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

MaterialTemplateManager::MaterialTemplateManager() {

}

void MaterialTemplateManager::get(const std::string &template_name, const MaterialPtr &material, std::function<void(const MaterialTemplateManager::Template &)> func) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_requirement((*it_require), (*it_template));
			}
			///////////
			// TESTME:
			// FIXME: Make sure that it's OK to set the properties for the material here before we run the func callback
			// FIXME: If Material relies on a value_changed() based scheme for testing validity and such up against the bound shader, etc
			// FIXME: then the Material actually have to set it's shader first, meaning the func should be called here, then set the properties.
			///////////
			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_properties((*it_require), material);
			}
			if(func) func((*it_template));

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
	MaterialTemplateParser::parse_templates(
		TextIO::load_contents(template_filename),
		[this](const MaterialTemplateManager::Template &t)
	{
		templates.push_back(t);
	});
}
