#pragma once

#include "../Utilities/RawImage.h"
#include "../../Core/Utilities/TextureFormat.h"

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace GM {
namespace Framework {

class ITextureIO; typedef std::shared_ptr<ITextureIO> ITextureIOPtr;
class TextureTemplateManager; typedef std::shared_ptr<TextureTemplateManager> TextureTemplateManagerPtr;
class TextureFormatTemplateManager; typedef std::shared_ptr<TextureFormatTemplateManager> TextureFormatTemplateManagerPtr;

typedef unsigned int TextureId;

class TextureManager
{
public:
	TextureManager(const ITextureIOPtr &texture_io);
	~TextureManager();

	const RawImage &get_or_create_image(const std::string &file_name);

	TextureId get(const std::string &texture_name);
	TextureId get_or_create(const std::string &texture_name);
	TextureId get_or_create(const std::string &texture_name, const Core::TextureFormat &format);
	TextureId get_or_create(const std::string &texture_name, const RawImage &image, const Core::TextureFormat &format);

	Core::TextureFormat *get_format(const std::string &format_name);
	Core::TextureFormat *get_or_create_format(const std::string &format_name, 
		const std::string &min_filter, 
		const std::string &mag_filter, 
		const std::string &wrap_s, 
		const std::string &wrap_t,
		// TODO: Add all other parameters TextureFormat can hold.
		bool generate_mipmap);

	void add_templates(const std::string &template_filename);
	void add_format_templates(const std::string &template_filename);
private:
	ITextureIOPtr texture_io;

	std::map<std::string, RawImage> loaded_images;

	std::map<RawImage*, std::vector<TextureId>> image_to_texture_deps;

	// TODO: still unclear
	std::map<TextureId, Core::TextureFormat*> texture_id_to_format;
	std::map<Core::TextureFormat*, TextureId> format_to_texture_id;

	//Associates a name with a texture.
	std::unordered_map<std::string, TextureId> name_to_texture;
	std::unordered_map<TextureId, std::string> texture_to_name;

	//Associates a name with a texture format.
	std::unordered_map<std::string, Core::TextureFormat*> name_to_texture_format;
	std::unordered_map<Core::TextureFormat*, std::string> texture_format_to_name;

	TextureTemplateManagerPtr template_manager;
	TextureFormatTemplateManagerPtr format_template_manager;
};

} // namespace Framework
} // namespace GM
