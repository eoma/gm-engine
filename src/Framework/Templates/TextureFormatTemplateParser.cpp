
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

void TextureFormatTemplateParser::parse_templates(const std::string &data, std::function<void(const TextureFormatTemplateManager::Template &)> func)
{
	if (func == nullptr)
		throw Exception("Func callback is required to use TemplateParser's parse_templates!");
		
	auto json_data = JsonValue::from_json(data);

	if (!json_data.is_array())
		throw Exception("Failed to load template data");

	for (size_t i = 0; i < json_data.get_size(); i++)
	{
		TextureFormatTemplateManager::Template t;

		auto json_entry = json_data[i];
		if (!json_entry.is_object())
			throw Exception("Array in json data should only hold objects");

		auto &json_members = json_entry.get_members();

		auto it = json_members.find("format");
		if (it == json_members.end())
			throw Exception("format is required");
		if (!it->second.is_string())
			throw Exception("format must be a string");
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

		it = json_members.find("type");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("type must be a string");
			t.type = it->second.to_string();
			if (t.type != "1d" &&
				t.type != "2d" &&
				t.type != "3d" &&
				t.type != "rect" &&
				t.type != "buffer" &&
				t.type != "cube" &&
				t.type != "1d_array" &&
				t.type != "2d_array" &&
				t.type != "cube_array" &&
				t.type != "2d_multisample" &&
				t.type != "2d_multisample_array") {
				throw clan::Exception("type must be one of the following: 1d, 2d, 3d, rect, buffer, cube, 1d_array, 2d_array, cube_array, 2d_multisample, 2d_multisample_array");
			}
		}

		it = json_members.find("min_filter");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("min_filter must be a string");
			t.min_filter = it->second.to_string();
		}

		it = json_members.find("mag_filter");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("mag_filter must be a string");
			t.mag_filter = it->second.to_string();
		}

		it = json_members.find("wrap_s");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("wrap_s must be a string");
			t.wrap_s = it->second.to_string();
		}

		it = json_members.find("wrap_t");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("wrap_t must be a string");
			t.wrap_t = it->second.to_string();
		}

		it = json_members.find("wrap_r");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("wrap_r must be a string");
			t.wrap_r = it->second.to_string();
		}

		it = json_members.find("depth_stencil_mode");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("depth_stencil_mode must be a string");
			t.depth_stencil_mode = it->second.to_string();
		}

		it = json_members.find("base_level");
		if (it != json_members.end())
		{
			if (!it->second.is_number())
				throw Exception("base_level must be a number");
			t.base_level = it->second.to_int();
		}

		it = json_members.find("compare_func");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("compare_func must be a string");
			t.compare_func = it->second.to_string();
		}

		it = json_members.find("compare_mode");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("compare_mode must be a string");
			t.compare_mode = it->second.to_string();
		}

		it = json_members.find("lod_bias");
		if (it != json_members.end())
		{
			if (!it->second.is_number())
				throw Exception("lod_bias must be a number");
			t.lod_bias = it->second.to_float();
		}

		it = json_members.find("min_lod");
		if (it != json_members.end())
		{
			if (!it->second.is_number())
				throw Exception("min_lod must be a number");
			t.min_lod = it->second.to_float();
		}

		it = json_members.find("max_lod");
		if (it != json_members.end())
		{
			if (!it->second.is_number())
				throw Exception("max_lod must be a number");
			t.max_lod = it->second.to_float();
		}

		it = json_members.find("max_level");
		if (it != json_members.end())
		{
			if (!it->second.is_number())
				throw Exception("max_level must be a number");
			t.max_level = it->second.to_int();
		}

		it = json_members.find("swizzle_r");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("swizzle_r must be a string");
			t.swizzle_r = it->second.to_string();
		}

		it = json_members.find("swizzle_g");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("swizzle_g must be a string");
			t.swizzle_g = it->second.to_string();
		}

		it = json_members.find("swizzle_b");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("swizzle_b must be a string");
			t.swizzle_b = it->second.to_string();
		}

		it = json_members.find("swizzle_a");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("swizzle_a must be a string");
			t.swizzle_a = it->second.to_string();
		}

		it = json_members.find("swizzle_rgba");
		if (it != json_members.end())
		{
			if (!it->second.is_string())
				throw Exception("swizzle_rgba must be a string");
			t.swizzle_rgba = it->second.to_string();
		}

		it = json_members.find("border_color");
		if (it != json_members.end())
		{
			if (!it->second.is_array() || it->second.get_items().size() != 4 || 
				!it->second.get_items()[0].is_number() || 
				!it->second.get_items()[1].is_number() || 
				!it->second.get_items()[2].is_number() || 
				!it->second.get_items()[3].is_number())
				throw Exception("border_color must be an array of 4 numbers");

			auto r = it->second.get_items()[0].to_float();
			auto g = it->second.get_items()[1].to_float();
			auto b = it->second.get_items()[2].to_float();
			auto a = it->second.get_items()[3].to_float();
			t.border_color = glm::vec4(r, g, b, a);
		}
		
		it = json_members.find("generate_mipmap");
		if (it != json_members.end())
		{
			if (!it->second.is_boolean())
				throw Exception("generate_mipmap must be a boolean");
			t.generate_mipmap = it->second.to_boolean();
		}

		func(t);
	}
}
