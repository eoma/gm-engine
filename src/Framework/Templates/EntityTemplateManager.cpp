
#include <GM/Framework/Templates/EntityTemplateManager.h>
#include <GM/Framework/Templates/EntityTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>
#include <GM/Framework/Utilities/ComponentSerializer.h>
#include <GM/Framework/Entity.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

EntityTemplateManager::EntityTemplateManager(const ComponentSerializerPtr &component_serializer)
: component_serializer(component_serializer)
{
}

void EntityTemplateManager::apply(const std::string &template_name, const EntityPtr &entity)
{
	for(auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if(StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for(auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_components((*it_require), entity);
			}

			for(auto it_component = it_template->components.begin(); it_component != it_template->components.end(); ++it_component)
			{
				component_serializer->create_and_add_component(entity, (*it_component), (*it_component));
			}

			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_properties((*it_require), entity);
			}

			for(auto it_property = it_template->properties.begin(); it_property != it_template->properties.end(); ++it_property)
			{
				PropertySerializer::create_and_add_property(*entity, (*it_property).type_id, (*it_property).name, (*it_property).value);
			}

			return;
		}
	}

	throw Exception("Unable to apply template " + template_name);
}

void EntityTemplateManager::apply_components(const std::string &template_name, const EntityPtr &entity) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_component = it_template->components.begin(); it_component != it_template->components.end(); ++it_component)
			{
				component_serializer->create_and_add_component(entity, (*it_component), (*it_component));
			}
		}
	}
}

void EntityTemplateManager::apply_properties(const std::string &template_name, const EntityPtr &entity) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_property = it_template->properties.begin(); it_property != it_template->properties.end(); ++it_property)
			{
				PropertySerializer::create_and_add_property(*entity, (*it_property).type_id, (*it_property).name, (*it_property).value);
			}
		}
	}
}

void EntityTemplateManager::add_templates(const std::string &template_filename)
{
	EntityTemplateParser::parse_templates(
		TextIO::load_contents(template_filename),
		[this](const EntityTemplateManager::Template &t)
		{
			templates.push_back(t);
		});
}
