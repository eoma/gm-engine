#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Templates/TextureTemplateManager.h"
#include "GM/Framework/Templates/TextureFormatTemplateManager.h"
#include "GM/Framework/IO/ITextureIO.h"

#include <ClanLib/core.h>

namespace GM {
namespace Framework {

TextureManager::TextureManager(const ITextureIOPtr &texture_io)
: texture_io(texture_io)
, loaded_images()
{
	template_manager = TextureTemplateManagerPtr(new TextureTemplateManager());
	format_template_manager = TextureFormatTemplateManagerPtr(new TextureFormatTemplateManager());
}

TextureManager::~TextureManager()
{
}

const RawImage &TextureManager::get_or_create_image(const std::string &file_name)
{
	auto iter = loaded_images.find(file_name);
	if (iter == loaded_images.end())
	{
		// TODO: check if file exists. Catch eventual exceptions. Check if image is empty
		if (!clan::FileHelp::file_exists(file_name)) {
			throw std::runtime_error("Texture does not exist!");
		}

		RawImage image = texture_io->load(file_name);

		if (image.get_data().empty() || image.get_width() < 1 || image.get_height() < 1)
		{
			throw std::runtime_error("The texture was not successfully loaded!");
		}

		iter = loaded_images.insert(std::make_pair(file_name, image)).first;
	}

	return iter->second;
}

TextureId TextureManager::get_or_create(const std::string &texture_name)
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
						t.min_filter, 
						t.mag_filter, 
						t.wrap_s, 
						t.wrap_t, 
						t.generate_mipmap);
				});
			}

			auto image = get_or_create_image(t.image);
			texture = get_or_create(texture_name, image, *format);

			name_to_texture[texture_name] = texture;
			texture_to_name[texture] = texture_name;
		});
	}

	// If we didn't find the proper template to create and cache this texture, we failed!
	if (!texture)
		throw clan::Exception(clan::string_format("Failed to get or create the texture %1.", texture_name));

	return texture;
}

TextureId TextureManager::get_or_create(const std::string &texture_name, const Core::TextureFormat &format)
{
	// TODO: Implement cache lookup for this name and make sure it uses the same format as specified.
	// TODO: Implement generating an empty texture based on format.
	throw clan::Exception("TextureId TextureManager::get_or_create(const std::string &texture_name, const Core::TextureFormat &format) is not yet implemented.");
}

TextureId TextureManager::get_or_create(const std::string &texture_name, const RawImage &image, const Core::TextureFormat &format)
{
	// TODO: Implement cache lookup for this name and make sure it uses the same image and format as specified.
	// TODO: Implement generating a texture based on raw image data and format.
	throw clan::Exception("TextureId TextureManager::get_or_create(const std::string &texture_name, const RawImage &image, const Core::TextureFormat &format) is not yet implemented.");
}

TextureId TextureManager::get(const std::string &texture_name)
{
	auto iter = name_to_texture.find(texture_name);
	if (iter == name_to_texture.end())
		return 0;

	return iter->second;
}

Core::TextureFormat *TextureManager::get_format(const std::string &format_name)
{
	auto iter = name_to_texture_format.find(format_name);
	if (iter == name_to_texture_format.end())
		return nullptr;

	return iter->second;
}

Core::TextureFormat *TextureManager::get_or_create_format(const std::string &format_name,
	const std::string &min_filter,
	const std::string &mag_filter,
	const std::string &wrap_s,
	const std::string &wrap_t,
	bool generate_mipmap)
{
	auto format = get_format(format_name);
	if (format)
		return format;

	format = Core::TextureFormat::create_texture_format_from_string(min_filter, mag_filter, wrap_s, wrap_t, generate_mipmap);
	name_to_texture_format[format_name] = format;
	texture_format_to_name[format] = format_name;

	return format;
}

} // namespace Framework
} // namespace GM
