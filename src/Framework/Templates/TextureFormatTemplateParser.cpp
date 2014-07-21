
#include <GM/Framework/Templates/TextureFormatTemplateParser.h>
#include <GM/Framework/Utilities/PropertySerializer.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

TextureFormatTemplateParser::TextureFormatTemplateParser()
{
}

void TextureFormatTemplateParser::parse_templates(const std::string &data/*, std::function<void(const TemplateManager::Template &)> func*/)
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

		auto it = json_members.find("format");
		if (it == json_members.end())
			throw Exception("format is required");
		if (!it->second.is_string())
			throw Exception("format must be a string");
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

		it = json_members.find("min_filter");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("min_filter must be a string");
			//t.min_filter = it->second.to_string();
		}

		it = json_members.find("mag_filter");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("mag_filter must be a string");
			//t.mag_filter = it->second.to_string();
		}

		it = json_members.find("wrap_s");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("wrap_s must be a string");
			//t.wrap_s = it->second.to_string();
		}

		it = json_members.find("wrap_t");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("wrap_t must be a string");
			//t.wrap_t = it->second.to_string();
		}
		
		it = json_members.find("generate_mipmap");
		if (it != json_members.end())
		{
			if (!it->second.is_boolean())
				throw Exception("generate_mipmap must be a boolean");
			//t.generate_mipmap = it->second.to_boolean();
		}

		// TODO: Add all remaining existing texture format parameters (as optional parameters)

		//func(t);
	}
}
