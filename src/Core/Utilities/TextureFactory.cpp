#include "GM/Core/Utilities/TextureFactory.h"

#include "GM/Core/GL/Texture.h"

#include "ClanLib/core.h"

#include <sstream>

namespace GM {
namespace Core {

TexturePtr TextureFactory::create(const TextureFormat &format, const std::vector<std::string> &image_paths, FileFetcherFunction &file_fetcher)
{
	auto texture = std::make_shared<Texture>(format.get_type());
	texture->bind();

	upload(texture, image_paths, file_fetcher);

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
		else if (ITextureParameter::is_type<std::vector<int>>(param))
		{
			// Should perform checks on the required number of parameters
			auto int_params = std::static_pointer_cast<TextureParameter<std::vector<int>>>(param);
			glTexParameteriv(format.get_type(), param->get_name(), int_params->get_param().data());
		}
		else if (ITextureParameter::is_type<std::vector<float>>(param))
		{
			// Should perform checks on the required number of parameters
			auto float_params = std::static_pointer_cast<TextureParameter<std::vector<float>>>(param);
			glTexParameterfv(format.get_type(), param->get_name(), float_params->get_param().data());
		}
	}

	if (format.is_generating_mipmap())
	{
		glGenerateMipmap(format.get_type());
	}

	texture->unbind();

	return texture;
}

TexturePtr TextureFactory::create(const TextureFormat &format, int width, int height, GLenum texture_format, GLenum data_type, std::shared_ptr<const std::vector<unsigned char>> data_ptr)
{
	auto texture = std::make_shared<Texture>(format.get_type());
	texture->bind();

	upload_single_image(texture, width, height, texture_format, data_type, data_ptr);

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
		else if (ITextureParameter::is_type<std::vector<int>>(param))
		{
			// Should perform checks on the required number of parameters
			auto int_params = std::static_pointer_cast<TextureParameter<std::vector<int>>>(param);
			glTexParameteriv(format.get_type(), param->get_name(), int_params->get_param().data());
		}
		else if (ITextureParameter::is_type<std::vector<float>>(param))
		{
			// Should perform checks on the required number of parameters
			auto float_params = std::static_pointer_cast<TextureParameter<std::vector<float>>>(param);
			glTexParameterfv(format.get_type(), param->get_name(), float_params->get_param().data());
		}
	}

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
		else if (ITextureParameter::is_type<std::vector<int>>(param))
		{
			// Should perform checks on the required number of parameters
			auto int_params = std::static_pointer_cast<TextureParameter<std::vector<int>>>(param);
			glTexParameteriv(format.get_type(), param->get_name(), int_params->get_param().data());
		}
		else if (ITextureParameter::is_type<std::vector<float>>(param))
		{
			// Should perform checks on the required number of parameters
			auto float_params = std::static_pointer_cast<TextureParameter<std::vector<float>>>(param);
			glTexParameterfv(format.get_type(), param->get_name(), float_params->get_param().data());
		}
	}

	if (format.is_generating_mipmap())
	{
		glGenerateMipmap(format.get_type());
	}

	texture->unbind();

	return texture;
}

void TextureFactory::upload(const TexturePtr &texture, const std::vector<std::string> &files, FileFetcherFunction &file_fetcher)
{
	if (texture->get_type() == GL_TEXTURE_2D)
	{
		if (files.size() != 1)
		{
			throw clan::Exception(clan::string_format("A 2d texture requires 1 image, (%1) were provided", files.size()));
		}

		upload_single_image(GL_TEXTURE_2D, files[0], file_fetcher);
	}
	else if (texture->get_type() == GL_TEXTURE_CUBE_MAP)
	{
		if (files.size() != 6)
		{
			throw clan::Exception(clan::string_format("A cube texture requires 6 images, (%1) were provided", files.size()));
		}

		for (unsigned int i = 0; i < 6; ++i)
		{
			upload_single_image(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, files[i], file_fetcher);
		}
	}
	else
	{
		std::stringstream str;
		str << "Unhandled texture type 0x" << std::hex << texture->get_type() << std::dec;
		throw clan::Exception(str.str());
	}
}

void TextureFactory::upload_single_image(const GLenum target, const std::string &name, FileFetcherFunction &file_fetcher)
{
	int width = 0;
	int height = 0;
	GLenum texture_format = 0;
	GLenum type = 0;

	std::shared_ptr<const std::vector<unsigned char>> imgdata = file_fetcher(name, width, height, 
		texture_format, type);

	glTexImage2D(target, 
	0, texture_format, width, height, 
	0, texture_format, type, imgdata->data());
}

void TextureFactory::upload_single_image(const TexturePtr &texture, int width, int height, GLenum texture_format, GLenum data_type, std::shared_ptr<const std::vector<unsigned char>> data_ptr)
{
	if (texture->get_type() != GL_TEXTURE_2D)
	{
		throw clan::Exception(clan::string_format("Only 2d texture is supported by this function overload, use version that loads image internally using vector of string paths"));
	}

	glTexImage2D(texture->get_type(),
		0, texture_format, width, height,
		0, texture_format, data_type, data_ptr->data());
}

} // namespace Core
} // namespace GM
