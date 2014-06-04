#include "precomp.h"
#include "template_manager.h"
#include "component_serializer.h"
#include "../server_gameobject.h"

using namespace clan;
using namespace Totem;

#pragma warning (disable:4701) // Avoid warning about uninitialized forced_type

TemplateManager::TemplateManager(const ComponentSerializerPtr &component_serializer)
: component_serializer(component_serializer)
{
}

void TemplateManager::apply(const std::string &template_name, const ServerGameObjectPtr &gameobject)
{
	for(auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if(StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for(auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply((*it_require), gameobject);
			}

			for(auto it_component = it_template->components.begin(); it_component != it_template->components.end(); ++it_component)
			{
				component_serializer->create_and_add_component(gameobject, (*it_component), (*it_component));
			}

			for(auto it_property = it_template->properties.begin(); it_property != it_template->properties.end(); ++it_property)
			{
				PropertySerializer::create_and_add_property(*gameobject, (*it_property).type_id, (*it_property).name, (*it_property).value);
			}

			return;
		}
	}

	throw Exception("Unable to apply template " + template_name);
}

void TemplateManager::add_templates(const std::string template_filename)
{
	auto data = File::read_text(template_filename);
	auto json_data = JsonValue::from_json(data);

	if(!json_data.is_array())
		throw Exception("Failed to load template data");

	for(size_t i = 0; i < json_data.get_size(); i++)
	{
		Template t;

		auto json_entry = json_data[i];
		if(!json_entry.is_object())
			throw Exception("Array in json data should only hold objects");

		auto &json_members = json_entry.get_members();
		
		auto it = json_members.find("template");
		if(it == json_members.end())
			throw Exception("Template is required");
		if(!it->second.is_string())
			throw Exception("Template must be a string");
		t.name = it->second.to_string();

//		std::cout << t.name << std::endl;

		it = json_members.find("requires");
		if(it != json_members.end()) {
			auto json_value = it->second;
			if(json_value.is_string())
			{
				auto require = json_value.to_string();
				t.requires.push_back(require);
//				std::cout << "Require: " << require << std::endl;
			}
			else if(json_value.is_array())
			{
				for(size_t j = 0; j < json_value.get_size(); j++)
				{
					if(json_value[j].is_string())
					{
						auto require = json_value[j].to_string();
						t.requires.push_back(require);
//						std::cout << "Require: " << require << std::endl;
					}
					else
					{
						throw Exception("Requires must be an array of strings: " + t.name);
					}
				}
			}
			else
			{
				throw Exception("Requires must be a string or an array: " + t.name);
			}
		}

		it = json_members.find("components");
		if(it != json_members.end()) 
		{
			auto json_value = it->second;
			if(json_value.is_string())
			{
				auto component = json_value.to_string();
				t.components.push_back(component);
//				std::cout << "Component: " << component << std::endl;
			}
			else if(json_value.is_array())
			{
				for(size_t j = 0; j < json_value.get_size(); j++)
				{
					if(json_value[j].is_string())
					{
						auto component = json_value[j].to_string();
						t.components.push_back(component);
//						std::cout << "Component: " << component << std::endl;
					}
					else
					{
						throw Exception("Components must be an array of strings: " + t.name);
					}
				}
			}
			else
			{
				throw Exception("Components must be a string or an array: " + t.name);
			}
		}

		it = json_members.find("properties");
		if(it != json_members.end()) 
		{
			auto json_value = it->second;
			if(!json_value.is_array())
				throw Exception("Properties must be an array: " + t.name);

			for(size_t j = 0; j < json_value.get_size(); j++)
			{
				if(json_value[j].is_object())
				{
					auto &json_members = json_value[j].get_members();
		
					Property p;

					bool has_forced_type = false;
					PropertySerializer::PropertyType forced_type;
					auto it_type = json_members.find("type");
					if(it_type != json_members.end())
					{
						if(!it_type->second.is_string())
							throw Exception("Property type must be a string");
		
						forced_type = PropertySerializer::get_property_type(it_type->second.to_string());
						has_forced_type = true;
					}

					auto it = json_members.find("name");
					if(it == json_members.end())
						throw Exception("Property name is required");
					if(!it->second.is_string())
						throw Exception("Property name must be a string");
					p.name = it->second.to_string();

					it = json_members.find("value");
					if(it == json_members.end())
						throw Exception("Property value is required");

					if(it->second.is_string() && !has_forced_type)
					{
						forced_type = PropertySerializer::TYPE_STRING;
					}
					else if(it->second.is_boolean() && !has_forced_type)
					{
						forced_type = PropertySerializer::TYPE_BOOL;
					}
					else if(it->second.is_number() && !has_forced_type)
					{
						forced_type = PropertySerializer::TYPE_FLOAT;
					}
					else if(!has_forced_type)
					{
						throw Exception(string_format("Unable to infer automatic type for property %1. Use 'type' field to specify a type.", p.name));
					}
					p.type_id = forced_type;

					if(forced_type == PropertySerializer::TYPE_STRING)
					{
						p.value = it->second.to_string();
					}
					else if(forced_type == PropertySerializer::TYPE_BOOL)
					{
						p.value = StringHelp::bool_to_text(it->second.to_boolean());
					}
					else if(forced_type == PropertySerializer::TYPE_FLOAT)
					{
						p.value = StringHelp::float_to_text(it->second.to_float());
					}
					else if(forced_type == PropertySerializer::TYPE_DOUBLE)
					{
						p.value = StringHelp::double_to_text(it->second.to_double());
					}
					else if(forced_type == PropertySerializer::TYPE_INT)
					{
						p.value = StringHelp::int_to_text(it->second.to_int());
					}
					else
						throw Exception("Type not implemented yet for property " + p.name);

					t.properties.push_back(p);
				}
				else
				{
					throw Exception("Properties must be an array of objects: " + t.name);
				}
			}
		}

		templates.push_back(t);
	}
}
