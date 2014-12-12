#include "GM/Core/Utilities/TextureFactory.h"

#include "GM/Core/GL/Texture.h"

#include "ClanLib/core.h"

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <algorithm>

namespace GM {
namespace Core {

TexturePtr TextureFactory::create(const TextureFormat &format, const std::vector<std::string> &image_paths, FileFetcherFunction &file_fetcher)
{
	std::vector<TextureData> images;
	images.reserve(image_paths.size());
	for (const std::string &path : image_paths)
	{
		images.push_back(fetch_image_data(path, file_fetcher));
	}

	return create(format, images);
}

TexturePtr TextureFactory::create(const TextureFormat &format, TextureData data)
{
	return create(format, std::vector<TextureData>{data});
}

TexturePtr TextureFactory::create(const TextureFormat &format, const std::vector<TextureData> &data)
{
	auto texture = std::make_shared<Texture>(format.get_type());
	texture->bind();

	if (texture->get_type() == GL_TEXTURE_2D)
	{
		if (data.size() != 1)
		{
			throw clan::Exception(clan::string_format("A 2d texture requires 1 image, (%1) were provided", data.size()));
		}

		make_immutable_storage(GL_TEXTURE_2D, format.is_generating_mipmap(), 
		                       data[0].internal_format, data[0].width, data[0].height);
		upload_single_image(GL_TEXTURE_2D, data[0]);
	}
	else if (texture->get_type() == GL_TEXTURE_CUBE_MAP)
	{
		if (data.size() != 6)
		{
			throw clan::Exception(clan::string_format("A cube texture requires 6 images, (%1) were provided", data.size()));
		}

		bool satisfied_image_sizes = std::all_of(++data.begin(), data.end(), 
			[&data](const TextureData &img) {
				return (data[0].width == img.width) && (data[0].height == img.height);
			});

		if (!satisfied_image_sizes)
		{
			throw clan::Exception("The six textures of a cube must the same dimensions!");
		}

		make_immutable_storage(GL_TEXTURE_CUBE_MAP, format.is_generating_mipmap(), 
		                       data[0].internal_format, data[0].width, data[0].height);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		for (unsigned int i = 0; i < data.size(); i++)
		{
			upload_single_image(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data[i]);
		}
	}

	set_parameters(texture, format);

	if (format.is_generating_mipmap())
	{
		glGenerateMipmap(format.get_type());
	}

	texture->unbind();

	return texture;
}

TexturePtr TextureFactory::create(const TextureFormat &format)
{
	auto texture = std::make_shared<Texture>(format.get_type());
	texture->bind();

	set_parameters(texture, format);

	if (format.is_generating_mipmap())
	{
		glGenerateMipmap(format.get_type());
	}

	texture->unbind();

	return texture;
}

void TextureFactory::set_parameters(const TexturePtr &texture, const TextureFormat &format)
{
	for (const ITextureParameterPtr &param : format.get_parameters())
	{
		if (ITextureParameter::is_type<int>(param))
		{
			auto int_param = std::static_pointer_cast<TextureParameter<int>>(param);
			glTexParameteri(format.get_type(), param->get_name(), int_param->get_param());
		}
		else if (ITextureParameter::is_type<float>(param))
		{
			auto float_param = std::static_pointer_cast<TextureParameter<float>>(param);
			glTexParameterf(format.get_type(), param->get_name(), float_param->get_param());
		}
		else if (ITextureParameter::is_type<glm::ivec4>(param))
		{
			auto int_params = std::static_pointer_cast<TextureParameter<glm::ivec4>>(param);
			glTexParameteriv(format.get_type(), param->get_name(), glm::value_ptr(int_params->get_param()));
		}
		else if (ITextureParameter::is_type<glm::vec4>(param))
		{
			auto float_params = std::static_pointer_cast<TextureParameter<glm::vec4>>(param);
			glTexParameterfv(format.get_type(), param->get_name(), glm::value_ptr(float_params->get_param()));
		}
	}
}

void TextureFactory::make_immutable_storage(GLenum target, bool mipmapped, GLenum internal_format, int width, int height)
{
	int numLevels = 1;
	if (mipmapped) // test if rectangle or buffer?
	{
		numLevels += floor(log2(std::max(width, height)));
	}

	// TODO: Maybe ensure that target is one GL_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE,
	// GL_PROXY_TEXTURE_2D, GL_PROXY_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_RECTANGLE 
	// or GL_PROXY_TEXTURE_CUBE_MAP?
	glTexStorage2D(target, numLevels, internal_format, width, height);
}

void TextureFactory::upload_single_image(const GLenum target, const TextureData &data)
{
	if (data.data_ptr != nullptr && data.data_ptr->size() > 0)
	{
		glTexSubImage2D(target, 0, 0, 0, data.width, data.height, data.texture_format, data.data_type, data.data_ptr->data());
	}
}

TextureFactory::TextureData TextureFactory::fetch_image_data(std::string image_name, FileFetcherFunction &file_fetcher)
{
	TextureData data;

	data.width = 0;
	data.height = 0;
	data.texture_format = 0;
	data.data_type = 0;

	data.data_ptr = file_fetcher(image_name, data.width, data.height, data.texture_format, data.data_type);

	// Assume all file based textures are unsigned 8-bit.
	data.internal_format = 0;
	if (data.texture_format == GL_RED) data.internal_format = GL_R8;
	else if (data.texture_format == GL_RG) data.internal_format = GL_RG8;
	else if (data.texture_format == GL_RGB) data.internal_format = GL_RGB8;
	else if (data.texture_format == GL_RGBA) data.internal_format = GL_RGBA8;

	return data;
}

} // namespace Core
} // namespace GM
