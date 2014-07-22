#include "GM/Core/Utilities/TextureFormat.h"
#include <ClanLib/core.h>
#include <algorithm>

namespace GM {
namespace Core {

TextureFormat::TextureFormat(const unsigned int type)
: type(type)
, make_mipmap(false)
{
};

int TextureFormat::compare(const TextureFormat &current, const TextureFormat &other)
{
	int result = current.type - other.type;

	if (result == 0) result = current.make_mipmap - other.make_mipmap;
	if (result == 0)
	{
		// test if current.parameters < other.parameters
		bool less_than = std::lexicographical_compare(
			current.parameters.begin(), current.parameters.end(),
			other.parameters.begin(), other.parameters.end(),
			std::owner_less<ITextureParameterPtr>());

		if (less_than)
		{
			result = -1;
		}
		else
		{
			// test if other.parameters < other.parameters
			less_than = std::lexicographical_compare(
				other.parameters.begin(), other.parameters.end(),
				current.parameters.begin(), current.parameters.end(),
				std::owner_less<ITextureParameterPtr>());

			if (less_than) result = 1;
		}
	}

	return result;
};

bool operator< (const TextureFormat &current, const TextureFormat &other)
{
	return TextureFormat::compare(current, other) ? true : false;
}

void TextureFormat::string_to_parameter(const std::string &value, float &result)
{
	throw clan::Exception(clan::string_format("The float parameter (%1) was not supported in TextureFormat when deserializing from string!", value));
}

void TextureFormat::string_to_parameter(const std::string &value, int &result)
{
	if (value == "linear_mipmap_linear") result = /*GL_LINEAR_MIPMAP_LINEAR*/1;
	else if (value == "linear") result = /*GL_LINEAR*/2;
	else if (value == "clamp_to_edge") result = /*GL_CLAMP_TO_EDGE*/3;
	else
		throw clan::Exception(clan::string_format("The int parameter (%1) was not supported in TextureFormat when deserializing from string!", value));
}

TextureFormat TextureFormat::create_texture2d_format(bool generate_mipmap, unsigned int wrap_mode)
{
	// FIXME: Uncomment when GL comes
	TextureFormat format(/*GL_TEXTURE_2D*/ 1);

	// FIXME: uncomment when GL comes
	//format.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//format.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//format.set_parameter(GL_TEXTURE_WRAP_S, wrap_mode);
	//format.set_parameter(GL_TEXTURE_WRAP_T, wrap_mode);

	format.set_generate_mipmap(generate_mipmap);

	return format;
}

TextureFormat *TextureFormat::create_texture_format_from_string(
	const std::string &min_filter,
	const std::string &mag_filter,
	const std::string &wrap_s,
	const std::string &wrap_t,
	bool generate_mipmap)
{
	// FIXME: Uncomment when GL comes
	TextureFormat *format = new TextureFormat(/*GL_TEXTURE_2D*/ 1);

	if (!min_filter.empty()) {
		int result;
		string_to_parameter(min_filter, result);
		//format->set_parameter(GL_TEXTURE_MIN_FILTER, result);
	}

	if (!mag_filter.empty()) {
		int result;
		string_to_parameter(mag_filter, result);
		//format->set_parameter(GL_TEXTURE_MAG_FILTER, result);
	}

	if (!wrap_s.empty()) {
		int result;
		string_to_parameter(wrap_s, result);
		//format->set_parameter(GL_TEXTURE_WRAP_S, result);
	}

	if (!wrap_t.empty()) {
		int result;
		string_to_parameter(wrap_t, result);
		//format->set_parameter(GL_TEXTURE_WRAP_T, result);
	}

	format->set_generate_mipmap(generate_mipmap);

	return format;
}

} // namespace Core
} // namespace GM
