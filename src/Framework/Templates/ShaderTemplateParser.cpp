
#include <GM/Framework/Templates/ShaderTemplateParser.h>
#include <GM/Framework/Utilities/PropertySerializer.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

ShaderTemplateParser::ShaderTemplateParser()
{
}

void ShaderTemplateParser::parse_templates(const std::string &data/*, std::function<void(const TemplateManager::Template &)> func*/)
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

		auto it = json_members.find("shader");
		if (it == json_members.end())
			throw Exception("shader is required");
		if (!it->second.is_string())
			throw Exception("shader must be a string");
		//t.name = it->second.to_string();

		//		std::cout << t.name << std::endl;

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

		it = json_members.find("vertex_shader");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("vertex_shader must be a string");
			//t.vertex_shader = it->second.to_string();
		}

		it = json_members.find("fragment_shader");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("fragment_shader must be a string");
			//t.fragment_shader = it->second.to_string();
		}

		it = json_members.find("geometry_shader");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("geometry_shader must be a string");
			//t.geometry_shader = it->second.to_string();
		}

		it = json_members.find("tesselation_control_shader");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("tesselation_control_shader must be a string");
			//t.tesselation_control_shader = it->second.to_string();
		}

		it = json_members.find("tesselation_evaluation_shader");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("tesselation_evaluation_shader must be a string");
			//t.tesselation_evaluation_shader = it->second.to_string();
		}

		it = json_members.find("compute_shader");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("compute_shader must be a string");
			//t.compute_shader = it->second.to_string();
		}

		//func(t);
	}
}
