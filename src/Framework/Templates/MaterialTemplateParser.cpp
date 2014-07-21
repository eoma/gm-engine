
#include <GM/Framework/Templates/MaterialTemplateParser.h>
#include <GM/Framework/Utilities/PropertySerializer.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

MaterialTemplateParser::MaterialTemplateParser()
{
}

void MaterialTemplateParser::parse_templates(const std::string &data/*, std::function<void(const TemplateManager::Template &)> func*/)
{
	/*if (func == nullptr)
		throw Exception("Func callback is required to use TemplateParser's parse_templates!");
		*/
	auto json_data = JsonValue::from_json(data);

	if (!json_data.is_array())
		throw Exception("Failed to load template data");

	for (size_t i = 0; i < json_data.get_size(); i++)
	{
		//TemplateManager::Template t;

		auto json_entry = json_data[i];
		if (!json_entry.is_object())
			throw Exception("Array in json data should only hold objects");

		auto &json_members = json_entry.get_members();

		auto it = json_members.find("material");
		if (it == json_members.end())
			throw Exception("material is required");
		if (!it->second.is_string())
			throw Exception("material must be a string");
		//t.name = it->second.to_string();

		//		std::cout << t.name << std::endl;

		// TODO: Requires must parse for types color and texture too...

		it = json_members.find("requires");
		if (it != json_members.end()) {
			auto json_value = it->second;
			if (json_value.is_string())
			{
				auto require = json_value.to_string();
				//t.requires.push_back(require);
				//				std::cout << "Require: " << require << std::endl;
			}
			else if (json_value.is_array())
			{
				for (size_t j = 0; j < json_value.get_size(); j++)
				{
					if (json_value[j].is_string())
					{
						auto require = json_value[j].to_string();
						//t.requires.push_back(require);
						//						std::cout << "Require: " << require << std::endl;
					}
					else
					{
						//throw Exception("Requires must be an array of strings: " + t.name);
					}
				}
			}
			else
			{
				//throw Exception("Requires must be a string or an array: " + t.name);
			}
		}

		it = json_members.find("shader");
		if (it == json_members.end())
			throw Exception("shader is required");
		if (!it->second.is_string())
			throw Exception("shader must be a string");
		//t.shader = it->second.to_string();

		it = json_members.find("uniforms");
		if (it != json_members.end())
		{
			auto json_value = it->second;
			/*if (!json_value.is_array())
				throw Exception("uniforms must be an array: " + t.name);*/

			for (size_t j = 0; j < json_value.get_size(); j++)
			{
				if (json_value[j].is_object())
				{
					auto &json_members = json_value[j].get_members();

					//MaterialManager::Uniform p;

					bool has_forced_type = false;
					PropertySerializer::PropertyType forced_type;
					auto it_type = json_members.find("type");
					if (it_type != json_members.end())
					{
						if (!it_type->second.is_string())
							throw Exception("Uniform type must be a string");

						forced_type = PropertySerializer::get_property_type(it_type->second.to_string());
						has_forced_type = true;
					}

					auto it = json_members.find("name");
					if (it == json_members.end())
						throw Exception("Uniform name is required");
					if (!it->second.is_string())
						throw Exception("Uniform name must be a string");
					//p.name = it->second.to_string();

					it = json_members.find("value");
					if (it == json_members.end())
						throw Exception("Uniform value is required");

					if (it->second.is_string() && !has_forced_type)
					{
						forced_type = PropertySerializer::TYPE_STRING;
					}
					else if (it->second.is_boolean() && !has_forced_type)
					{
						forced_type = PropertySerializer::TYPE_BOOL;
					}
					else if (it->second.is_number() && !has_forced_type)
					{
						forced_type = PropertySerializer::TYPE_FLOAT;
					}
					else if (!has_forced_type)
					{
						//throw Exception(string_format("Unable to infer automatic type for uniform %1. Use 'type' field to specify a type.", p.name));
					}
					//p.type_id = forced_type;

					if (forced_type == PropertySerializer::TYPE_STRING)
					{
						//p.value = it->second.to_string();
					}
					else if (forced_type == PropertySerializer::TYPE_BOOL)
					{
						//p.value = StringHelp::bool_to_text(it->second.to_boolean());
					}
					else if (forced_type == PropertySerializer::TYPE_FLOAT)
					{
						//p.value = StringHelp::float_to_text(it->second.to_float());
					}
					else if (forced_type == PropertySerializer::TYPE_DOUBLE)
					{
						//p.value = StringHelp::double_to_text(it->second.to_double());
					}
					else if (forced_type == PropertySerializer::TYPE_INT)
					{
						//p.value = StringHelp::int_to_text(it->second.to_int());
					}
					else if (forced_type == PropertySerializer::TYPE_VEC2)
					{
						if (!it->second.is_array())
							throw Exception("A vec2 type requires an array!");

						if (it->second.get_size() != 2)
							throw Exception("A vec2 type's array must be of size 2!");

						if (!it->second[0].is_number() || !it->second[1].is_number())
							throw Exception("A vec2 type requires an array of numbers!");

						/*p.value =
							StringHelp::float_to_text(it->second[0].to_float()) + " " +
							StringHelp::float_to_text(it->second[1].to_float());*/
					}
					else if (forced_type == PropertySerializer::TYPE_VEC3)
					{
						if (!it->second.is_array())
							throw Exception("A vec3 type requires an array!");

						if (it->second.get_size() != 3)
							throw Exception("A vec3 type's array must be of size 3!");

						if (!it->second[0].is_number() || !it->second[1].is_number() || !it->second[2].is_number())
							throw Exception("A vec3 type requires an array of numbers!");

						/*p.value =
							StringHelp::float_to_text(it->second[0].to_float()) + " " +
							StringHelp::float_to_text(it->second[1].to_float()) + " " +
							StringHelp::float_to_text(it->second[2].to_float());*/
					}
					else if (forced_type == PropertySerializer::TYPE_VEC4)
					{
						if (!it->second.is_array())
							throw Exception("A vec4 type requires an array!");

						if (it->second.get_size() != 4)
							throw Exception("A vec4 type's array must be of size 4!");

						if (!it->second[0].is_number() || !it->second[1].is_number() || !it->second[2].is_number() || !it->second[3].is_number())
							throw Exception("A vec4 type requires an array of numbers!");

						/*p.value =
							StringHelp::float_to_text(it->second[0].to_float()) + " " +
							StringHelp::float_to_text(it->second[1].to_float()) + " " +
							StringHelp::float_to_text(it->second[2].to_float()) + " " +
							StringHelp::float_to_text(it->second[3].to_float());*/
					}
					else if (forced_type == PropertySerializer::TYPE_QUAT)
					{
						if (!it->second.is_array())
							throw Exception("A quat type requires an array!");

						if (it->second.get_size() != 4)
							throw Exception("A quat type's array must be of size 4!");

						if (!it->second[0].is_number() || !it->second[1].is_number() || !it->second[2].is_number() || !it->second[3].is_number())
							throw Exception("A quat type requires an array of numbers!");

						/*p.value =
							StringHelp::float_to_text(it->second[0].to_float()) + " " +
							StringHelp::float_to_text(it->second[1].to_float()) + " " +
							StringHelp::float_to_text(it->second[2].to_float()) + " " +
							StringHelp::float_to_text(it->second[3].to_float());*/
					}
					else if (forced_type == PropertySerializer::TYPE_COLOR)
					{
						//p.value = it->second.to_string();
					}
					else if (forced_type == PropertySerializer::TYPE_TEXTURE_NAME)
					{
						//p.value = it->second.to_string();
					}
					/*else
						throw Exception("Type not implemented yet for property " + p.name);*/

					//t.properties.push_back(p);
				}
				else
				{
					//throw Exception("Properties must be an array of objects: " + t.name);
				}
			}
		}

		//func(t);
	}
}
