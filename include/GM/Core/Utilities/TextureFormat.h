#pragma once

#include "TextureParameter.h"

#include <string>
#include <vector>

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

	const std::vector<std::string> &get_image_files() const { return image_files; }

	const std::vector<ITextureParameterPtr> &get_parameters() const { return parameters; }

	template<class T>
	std::vector<std::shared_ptr<TextureParameter<T>>> get_parameters_of_type() const;

	static int compare(const TextureFormat &current, const TextureFormat &other);

public:
	// Static convenience functions dealing with creation of formats
	static TextureFormat create_texture2d_format(const std::string &name);

protected:
	// TODO: A bit unclear a bout these, why should they be protected?
	TextureFormat(const unsigned int type);

	void set_generate_mipmap(bool value) { make_mipmap = value; }

	void add_image_file(const std::string &file_name) { image_files.push_back(file_name); }

	template<class T>
	void set_parameter(unsigned int param_name, T param); 

	void set_parameter(unsigned int param_name, float param) { set_parameter<float>(param_name, param); }
	void set_parameter(unsigned int param_name, int param) { set_parameter<int>(param_name, param); }
	void set_parameter(unsigned int param_name, std::vector<float> param) { set_parameter<std::vector<float>>(param_name, param); }
	void set_parameter(unsigned int param_name, std::vector<int> param) { set_parameter<std::vector<int>>(param_name, param); }

protected:
	// texture format type
	const unsigned int type;

	// should we create a mipmap for the(se) texture(s)?
	bool make_mipmap;

	// Can _not_ be sorted, order must be preserved
	std::vector<std::string> image_files;

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
