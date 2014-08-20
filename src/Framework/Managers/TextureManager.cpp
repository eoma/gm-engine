#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Templates/TextureTemplateManager.h"
#include "GM/Framework/Templates/TextureFormatTemplateManager.h"
#include "GM/Framework/IO/IImageIO.h"

#include "GM/Core/GL/Texture.h"
#include "GM/Core/Utilities/TextureFactory.h"

#include <ClanLib/core.h>

namespace GM {
namespace Framework {

TextureManager::TextureManager(const IImageIOPtr &image_io)
: image_io(image_io)
, loaded_images()
{
	template_manager = TextureTemplateManagerPtr(new TextureTemplateManager());
	format_template_manager = TextureFormatTemplateManagerPtr(new TextureFormatTemplateManager());
}

TextureManager::~TextureManager()
{

}

const RawImagePtr &TextureManager::get_or_create_image(const std::string &file_name)
{
	auto iter = loaded_images.find(file_name);
	if (iter == loaded_images.end())
	{
		if (!clan::FileHelp::file_exists(texture_path + "/" + file_name)) {
			throw clan::Exception(clan::string_format("Texture image (%1) does not exist!", file_name));
		}

		RawImagePtr image = std::make_shared<RawImage>(image_io->load(texture_path + "/" + file_name));

		if (image->get_data().empty() || image->get_width() < 1 || image->get_height() < 1)
		{
			throw clan::Exception(clan::string_format("The texture image (%1) was not successfully loaded!", file_name));
		}

		iter = loaded_images.insert(std::make_pair(file_name, image)).first;
	}

	return iter->second;
}

Core::TexturePtr TextureManager::get_or_create(const std::string &texture_name)
{
	// First, test if the name has been cached.
	auto texture = get(texture_name);

	// If not cached, let's see if there is a template description for this name.
	if (!texture) {
		template_manager->get(texture_name, [this, texture_name, &texture](const TextureTemplateManager::Template &t) {

			// First, we test if the format is already cached.
			auto format = get_format(t.format);

			// And if not, we try to load it from a template description.
			if (!format) {
				format_template_manager->get(t.format, [this, &format](const TextureFormatTemplateManager::Template &t) {
					format = get_or_create_format(t.name, 
						t.type,
						t.min_filter, 
						t.mag_filter, 
						t.wrap_s, 
						t.wrap_t, 
						t.wrap_r,
						t.depth_stencil_mode,
						t.base_level,
						t.compare_func,
						t.compare_mode,
						t.lod_bias,
						t.min_lod,
						t.max_lod,
						t.max_level,
						t.swizzle_r,
						t.swizzle_g,
						t.swizzle_b,
						t.swizzle_a,
						t.swizzle_rgba,
						t.border_color,
						t.generate_mipmap,
						t.gl_texture_format);
				});
			}

			std::vector<RawImagePtr> images;
			for (auto image_filename : t.images) {
				auto image = get_or_create_image(image_filename);
				images.push_back(image);
			}

			texture = get_or_create(texture_name, images, *format);
		});
	}

	return texture;
}

Core::TexturePtr TextureManager::get_or_create(const std::string &texture_name, const Core::TextureFormat &format)
{
	// First, test if the name has been cached.
	auto texture = get(texture_name);
	if (texture) {
		return texture;
	}

	// Implement generating an empty texture based on format.
	texture = Core::TextureFactory::create(format);
	if (!texture) {
		throw clan::Exception(clan::string_format("Failed to get or create the texture %1.", texture_name));
	}

	name_to_texture[texture_name] = texture;
	texture_to_name[texture] = texture_name;

	return texture;
}

Core::TexturePtr TextureManager::get_or_create(const std::string &texture_name, const RawImage &image, const Core::TextureFormat &format)
{
	// First, test if the name has been cached.
	auto texture = get(texture_name);
	if (texture) {
		return texture;
	}

	
	GLenum gl_texture_format;
	
	// Have the user set a forced texture format?
	if (format.has_gl_texture_format()) {
		gl_texture_format = format.get_gl_texture_format();
	}
	// Implement generating a texture based on raw image data and format.
	else {
		int channels = image.get_num_channels();
		if (channels == 1) gl_texture_format = GL_RED;
		else if (channels == 2) gl_texture_format = GL_RG;
		else if (channels == 3) gl_texture_format = GL_RGB;
		else if (channels == 4) gl_texture_format = GL_RGBA;
		else gl_texture_format = GL_RGB;
	}

	texture = Core::TextureFactory::create(format, Core::TextureFactory::TextureData{
		(int)image.get_width(),
		(int)image.get_height(),
		gl_texture_format,
		(GLenum)GL_UNSIGNED_BYTE,
		image.get_dataptr()
	});
	if (!texture) {
		throw clan::Exception(clan::string_format("Failed to get or create the texture %1.", texture_name));
	}

	name_to_texture[texture_name] = texture;
	texture_to_name[texture] = texture_name;

	return texture;
}

Core::TexturePtr TextureManager::get_or_create(const std::string &texture_name, const std::vector<RawImagePtr> &images, const Core::TextureFormat &format)
{
	// First, test if the name has been cached.
	auto texture = get(texture_name);
	if (texture) {
		return texture;
	}

	std::vector<Core::TextureFactory::TextureData> data;
	for (auto image : images) {

		// Implement generating a texture based on raw image data and format.
		GLenum gl_texture_format;
		int channels = image->get_num_channels();
		if (channels == 1) gl_texture_format = GL_RED;
		else if (channels == 2) gl_texture_format = GL_RG;
		else if (channels == 3) gl_texture_format = GL_RGB;
		else if (channels == 4) gl_texture_format = GL_RGBA;
		else gl_texture_format = GL_RGB;

		data.push_back(Core::TextureFactory::TextureData { 
			(int)image->get_width(), 
			(int)image->get_height(), 
			gl_texture_format, 
			(GLenum)GL_UNSIGNED_BYTE, 
			image->get_dataptr() 
		});
	}

	texture = Core::TextureFactory::create(format, data);

	if (!texture) {
		throw clan::Exception(clan::string_format("Failed to get or create the texture %1.", texture_name));
	}

	name_to_texture[texture_name] = texture;
	texture_to_name[texture] = texture_name;

	return texture;
}

Core::TexturePtr TextureManager::get(const std::string &texture_name)
{
	auto iter = name_to_texture.find(texture_name);
	if (iter == name_to_texture.end())
		return 0;

	return iter->second;
}

Core::TextureFormatPtr TextureManager::get_format(const std::string &format_name)
{
	auto iter = name_to_texture_format.find(format_name);
	if (iter == name_to_texture_format.end())
		return nullptr;

	return iter->second;
}

Core::TextureFormatPtr TextureManager::get_or_create_format(const std::string &format_name,
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
	auto format = get_format(format_name);
	if (format)
		return format;

	format = Core::TextureFormat::create_texture_format_from_string(
		type,
		min_filter, 
		mag_filter, 
		wrap_s, 
		wrap_t, 
		wrap_r,
		depth_stencil_mode,
		base_level,
		compare_func,
		compare_mode,
		lod_bias,
		min_lod,
		max_lod,
		max_level,
		swizzle_r,
		swizzle_g,
		swizzle_b,
		swizzle_a,
		swizzle_rgba,
		border_color,
		generate_mipmap,
		gl_texture_format);
	name_to_texture_format[format_name] = format;
	texture_format_to_name[format] = format_name;

	return format;
}

void TextureManager::add_templates(const std::string &template_filename)
{
	template_manager->add_templates(template_filename);
}

void TextureManager::add_format_templates(const std::string &template_filename)
{
	format_template_manager->add_templates(template_filename);
}

} // namespace Framework
} // namespace GM
