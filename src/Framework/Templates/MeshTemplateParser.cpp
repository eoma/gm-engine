
#include <GM/Framework/Templates/MeshTemplateParser.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

MeshTemplateParser::MeshTemplateParser()
{
}

void MeshTemplateParser::parse_templates(const std::string &data, std::function<void(const MeshTemplateManager::Template &)> func)
{
	if (func == nullptr)
		throw Exception("Func callback is required to use MeshTemplateParser's parse_templates!");
		
	auto json_data = JsonValue::from_json(data);

	if (!json_data.is_array())
		throw Exception("Failed to load template data");

	for (size_t i = 0; i < json_data.get_size(); i++)
	{
		MeshTemplateManager::Template t;

		auto json_entry = json_data[i];
		if (!json_entry.is_object())
			throw Exception("Array in json data should only hold objects");

		auto &json_members = json_entry.get_members();

		auto it = json_members.find("mesh");
		if (it == json_members.end())
			throw Exception("mesh is required");
		if (!it->second.is_string())
			throw Exception("mesh must be a string");
		t.name = it->second.to_string();

		//		std::cout << t.name << std::endl;

		it = json_members.find("requires");
		if (it != json_members.end()) {
			auto json_value = it->second;
			if (json_value.is_string())
			{
				auto require = json_value.to_string();
				t.requires.push_back(require);
				//				std::cout << "Require: " << require << std::endl;
			}
			else if (json_value.is_array())
			{
				for (size_t j = 0; j < json_value.get_size(); j++)
				{
					if (json_value[j].is_string())
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

		it = json_members.find("filename");
		if (it == json_members.end())
			throw Exception("filename is required");
		if (!it->second.is_string())
			throw Exception("filename must be a string");
		t.filename = it->second.to_string();

		it = json_members.find("mesh_index");
		if (it == json_members.end())
			throw Exception("mesh_index is required");
		if (!it->second.is_number())
			throw Exception("mesh_index must be a number");
		t.mesh_index = it->second.to_int();

		func(t);
	}
}
