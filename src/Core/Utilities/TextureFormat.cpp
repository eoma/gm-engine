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

unsigned int TextureFormat::string_to_type(const std::string &value)
{
	if (value == "1d") {
		return GL_TEXTURE_1D;
	}	
	if (value == "2d") {
		return GL_TEXTURE_2D;
	}	
	if (value == "3d") {
		return GL_TEXTURE_3D;
	}	
	if (value == "rect") {
		return GL_TEXTURE_RECTANGLE;
	}	
	if (value == "buffer") {
		return GL_TEXTURE_BUFFER;
	}	
	if (value == "cube") {
		return GL_TEXTURE_CUBE_MAP;
	}	
	if (value == "1d_array") {
		return GL_TEXTURE_1D_ARRAY;
	}	
	if (value == "2d_array") {
		return GL_TEXTURE_2D_ARRAY;
	}	
	if (value == "cube_array") {
		return GL_TEXTURE_CUBE_MAP_ARRAY;
	}	
	if (value == "2d_multisample") {
		return GL_TEXTURE_2D_MULTISAMPLE;
	}	
	if (value == "2d_multisample_array") {
		return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	}

	// If all else fails, default to 2d.
	return GL_TEXTURE_2D;
}

void TextureFormat::string_to_parameter(const std::string &value, int &result)
{
	if (value == "nearest") result = GL_NEAREST;
	else if (value == "linear") result = GL_LINEAR;
	else if (value == "nearest_mipmap_nearest") result = GL_NEAREST_MIPMAP_NEAREST;
	else if (value == "linear_mipmap_nearest") result = GL_LINEAR_MIPMAP_NEAREST;
	else if (value == "nearest_mipmap_linear") result = GL_NEAREST_MIPMAP_LINEAR;
	else if (value == "linear_mipmap_linear") result = GL_LINEAR_MIPMAP_LINEAR;

	else if (value == "clamp_to_edge") result = GL_CLAMP_TO_EDGE;
	else if (value == "clamp_to_border") result = GL_CLAMP_TO_BORDER;
	else if (value == "mirrored_repeat") result = GL_MIRRORED_REPEAT;
	else if (value == "repeat") result = GL_REPEAT;
	//else if (value == "mirror_clamp_to_edge") result = GL_MIRROR_CLAMP_TO_EDGE;

	else if (value == "depth") result = GL_DEPTH_COMPONENT;
	else if (value == "stencil") result = GL_STENCIL_INDEX;

	else if (value == "stencil") result = GL_STENCIL_INDEX;

	else if (value == "lequal") result = GL_LEQUAL;
	else if (value == "gequal") result = GL_GEQUAL;
	else if (value == "less") result = GL_LESS;
	else if (value == "greater") result = GL_GREATER;
	else if (value == "equal") result = GL_EQUAL;
	else if (value == "notequal") result = GL_NOTEQUAL;
	else if (value == "always") result = GL_ALWAYS;
	else if (value == "never") result = GL_NEVER;

	else if (value == "compare") result = GL_COMPARE_REF_TO_TEXTURE;
	else if (value == "none") result = GL_NONE;

	else if (value == "red") result = GL_RED;
	else if (value == "green") result = GL_GREEN;
	else if (value == "blue") result = GL_BLUE;
	else if (value == "alpha") result = GL_ALPHA;
	else if (value == "zero") result = GL_ZERO;
	else if (value == "one") result = GL_ONE;

	else
		throw clan::Exception(clan::string_format("The int parameter (%1) was not supported in TextureFormat when deserializing from string!", value));
}

GLenum TextureFormat::string_to_gl_texture_format(const std::string &value) {
	if (value == "r") return GL_RED;
	if (value == "g") return GL_GREEN;
	if (value == "b") return GL_BLUE;
	if (value == "a") return GL_ALPHA;

	if (value == "rg") return GL_RG;
	if (value == "rgb") return GL_RGB;
	if (value == "rgba") return GL_RGBA;

	if (value == "bgr") return GL_BGR;
	if (value == "bgra") return GL_BGRA;

	if (value == "rg16f") return GL_RG16F;
	if (value == "rgb16f") return GL_RGB16F;
	if (value == "rgba16f") return GL_RGBA16F;

	if (value == "rg32f") return GL_RG32F;
	if (value == "rgb32f") return GL_RGB32F;
	if (value == "rgba32f") return GL_RGBA32F;

	if (value == "srgb") return GL_SRGB;
	if (value == "srgba") return GL_SRGB_ALPHA;

	if (value == "depth_component16") return GL_DEPTH_COMPONENT16;
	if (value == "depth_component24") return GL_DEPTH_COMPONENT24;
	if (value == "depth_component32") return GL_DEPTH_COMPONENT32;
	if (value == "depth_component32f") return GL_DEPTH_COMPONENT32F;

	// TODO: Add all formats in the current opengl spec... yeah... there's many :-P
}

TextureFormatPtr TextureFormat::create_texture2d_format(bool generate_mipmap, unsigned int wrap_mode)
{
	TextureFormatPtr format(new TextureFormat(GL_TEXTURE_2D));

	format->set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	format->set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	format->set_parameter(GL_TEXTURE_WRAP_S, wrap_mode);
	format->set_parameter(GL_TEXTURE_WRAP_T, wrap_mode);

	format->set_generate_mipmap(generate_mipmap);

	return format;
}

TextureFormatPtr TextureFormat::create_texture_format_from_string(
	const std::string &type,
	const std::string &min_filter,
	const std::string &mag_filter,
	const std::string &wrap_s,
	const std::string &wrap_t,
	const std::string &wrap_r,
	const std::string &depth_stencil_mode,
	int base_level,
	const std::string &compare_func,
	const std::string &compare_mode,
	float lod_bias,
	float min_lod,
	float max_lod,
	int max_level,
	const std::string &swizzle_r,
	const std::string &swizzle_g,
	const std::string &swizzle_b,
	const std::string &swizzle_a,
	const std::string &swizzle_rgba,
	const glm::vec4 &border_color,
	bool generate_mipmap,
	const std::string &gl_texture_format)
{
	TextureFormatPtr format(new TextureFormat(string_to_type(type)));

	if (!min_filter.empty()) {
		int result;
		string_to_parameter(min_filter, result);
		format->set_parameter(GL_TEXTURE_MIN_FILTER, result);
	}

	if (!mag_filter.empty()) {
		int result;
		string_to_parameter(mag_filter, result);
		format->set_parameter(GL_TEXTURE_MAG_FILTER, result);
	}

	if (!wrap_s.empty()) {
		int result;
		string_to_parameter(wrap_s, result);
		format->set_parameter(GL_TEXTURE_WRAP_S, result);
	}

	if (!wrap_t.empty()) {
		int result;
		string_to_parameter(wrap_t, result);
		format->set_parameter(GL_TEXTURE_WRAP_T, result);
	}

	if (!wrap_r.empty()) {
		int result;
		string_to_parameter(wrap_r, result);
		format->set_parameter(GL_TEXTURE_WRAP_R, result);
	}

	if (!depth_stencil_mode.empty()) {
		int result;
		string_to_parameter(depth_stencil_mode, result);
		format->set_parameter(GL_DEPTH_STENCIL_TEXTURE_MODE, result);
	}

	if (!compare_func.empty()) {
		int result;
		string_to_parameter(compare_func, result);
		format->set_parameter(GL_TEXTURE_COMPARE_FUNC, result);
	}

	if (!compare_mode.empty()) {
		int result;
		string_to_parameter(compare_mode, result);
		format->set_parameter(GL_TEXTURE_COMPARE_MODE, result);
	}

	if (!swizzle_r.empty()) {
		int result;
		string_to_parameter(swizzle_r, result);
		format->set_parameter(GL_TEXTURE_SWIZZLE_R, result);
	}

	if (!swizzle_g.empty()) {
		int result;
		string_to_parameter(swizzle_g, result);
		format->set_parameter(GL_TEXTURE_SWIZZLE_G, result);
	}

	if (!swizzle_b.empty()) {
		int result;
		string_to_parameter(swizzle_b, result);
		format->set_parameter(GL_TEXTURE_SWIZZLE_B, result);
	}

	if (!swizzle_a.empty()) {
		int result;
		string_to_parameter(swizzle_a, result);
		format->set_parameter(GL_TEXTURE_SWIZZLE_A, result);
	}

	if (!swizzle_rgba.empty()) {
		auto rgba = clan::StringHelp::split_text(swizzle_rgba, " ", false);
		if (rgba.size() == 4) {
			for (unsigned int i = 0; i < rgba.size(); i++) {
				int result;
				string_to_parameter(rgba[i], result);
				format->set_parameter(GL_TEXTURE_SWIZZLE_R+i, result);
			}
		}
	}

	if (!gl_texture_format.empty()) {
		format->gl_texture_format = string_to_gl_texture_format(gl_texture_format);
	}
	else {
		format->gl_texture_format = GL_NONE;
	}

	format->set_parameter(GL_TEXTURE_BASE_LEVEL, base_level);
	format->set_parameter(GL_TEXTURE_LOD_BIAS, lod_bias);
	format->set_parameter(GL_TEXTURE_MIN_LOD, min_lod);
	format->set_parameter(GL_TEXTURE_MAX_LOD, max_lod);
	format->set_parameter(GL_TEXTURE_MAX_LEVEL, max_level);

	format->set_parameter(GL_TEXTURE_BORDER_COLOR, border_color);

	format->set_generate_mipmap(generate_mipmap);

	return format;
}

} // namespace Core
} // namespace GM
