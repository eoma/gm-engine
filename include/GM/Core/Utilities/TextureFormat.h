#pragma once

#include "TextureParameter.h"

#include <string>
#include <vector>

#include <GL/gl3w.h>
#include <glm/glm.hpp>

namespace GM {
namespace Core {

/**
 * Texture format is just a container of parameters..
 * You should add specializations that derive from this container to make eg. 2D textures or cube maps
 */
class TextureFormat
{
public:
	virtual ~TextureFormat() {};
	
	unsigned int get_type() const { return type; }

	bool is_generating_mipmap() const { return make_mipmap; }

	const std::vector<ITextureParameterPtr> &get_parameters() const { return parameters; }

	template<class T>
	std::vector<std::shared_ptr<TextureParameter<T>>> get_parameters_of_type() const;

	static int compare(const TextureFormat &current, const TextureFormat &other);

public:
	// Static convenience functions dealing with creation of formats

	static TextureFormat create_texture2d_format(bool generate_mipmap = true, unsigned int wrap_mode = GL_CLAMP_TO_EDGE);
	static TextureFormat *create_texture_format_from_string(
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
		bool generate_mipmap);

	static void string_to_parameter(const std::string &value, float &result);
	static void string_to_parameter(const std::string &value, int &result);
	static unsigned int string_to_type(const std::string &value);

protected:
	// TODO: A bit unclear a bout these, why should they be protected?
	TextureFormat(const unsigned int type);

	void set_generate_mipmap(bool value) { make_mipmap = value; }

	template<class T>
	void set_parameter(unsigned int param_name, T param); 

	void set_parameter(unsigned int param_name, float param) { set_parameter<float>(param_name, param); }
	void set_parameter(unsigned int param_name, int param) { set_parameter<int>(param_name, param); }
	void set_parameter(unsigned int param_name, glm::vec4 param) { set_parameter<glm::vec4>(param_name, param); }
	void set_parameter(unsigned int param_name, glm::ivec4 param) { set_parameter<glm::ivec4>(param_name, param); }

protected:
	// texture format type
	const unsigned int type;

	// should we create a mipmap for the(se) texture(s)?
	bool make_mipmap;

	// Ordered list of parameters.
	std::vector<ITextureParameterPtr> parameters;
};

bool operator< (const TextureFormat &current, const TextureFormat &other);

// Implementations

template<class T>
std::vector<std::shared_ptr<TextureParameter<T>>> TextureFormat::get_parameters_of_type() const
{
	std::vector<std::shared_ptr<TextureParameter<T>>> type_parameters;
	for (auto parameter : parameters)
	{
		if (ITextureParameter::is_type<T>(parameter))
			type_parameters.push_back(std::static_pointer_cast<TextureParameter<T>>(parameter));
	}

	return type_parameters;
}
	
template<class T>
void TextureFormat::set_parameter(unsigned int param_name, T param)
{ 
	auto texture_parameter = std::make_shared<TextureParameter<T>>(param_name, param);
	parameters.push_back(std::static_pointer_cast<ITextureParameter>(texture_parameter));
}

} // namespace Core
} // namespace GM
