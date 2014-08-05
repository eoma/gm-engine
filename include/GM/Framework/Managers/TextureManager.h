#pragma once

#include "../Utilities/RawImage.h"
#include "../../Core/Utilities/TextureFormat.h"

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace GM {
namespace Core {
	class Texture; typedef std::shared_ptr<Texture> TexturePtr;
}
namespace Framework {

class ITextureIO; typedef std::shared_ptr<ITextureIO> ITextureIOPtr;
class TextureTemplateManager; typedef std::shared_ptr<TextureTemplateManager> TextureTemplateManagerPtr;
class TextureFormatTemplateManager; typedef std::shared_ptr<TextureFormatTemplateManager> TextureFormatTemplateManagerPtr;

class TextureManager
{
public:
	TextureManager(const ITextureIOPtr &texture_io);
	~TextureManager();

	void set_texture_path(const std::string &texture_path) { this->texture_path = texture_path; }

	const RawImage &get_or_create_image(const std::string &file_name);

	Core::TexturePtr get(const std::string &texture_name);
	Core::TexturePtr get_or_create(const std::string &texture_name);
	Core::TexturePtr get_or_create(const std::string &texture_name, const Core::TextureFormat &format);
	Core::TexturePtr get_or_create(const std::string &texture_name, const RawImage &image, const Core::TextureFormat &format);

	Core::TextureFormat *get_format(const std::string &format_name);
	Core::TextureFormat *get_or_create_format(const std::string &format_name, 
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

	void add_templates(const std::string &template_filename);
	void add_format_templates(const std::string &template_filename);
private:
	ITextureIOPtr texture_io;

	std::map<std::string, RawImage> loaded_images;

	std::map<RawImage*, std::vector<Core::TexturePtr>> image_to_texture_deps;

	// TODO: still unclear
	std::map<Core::TexturePtr, Core::TextureFormat*> texture_id_to_format;
	std::map<Core::TextureFormat*, Core::TexturePtr> format_to_texture_id;

	//Associates a name with a texture.
	std::unordered_map<std::string, Core::TexturePtr> name_to_texture;
	std::unordered_map<Core::TexturePtr, std::string> texture_to_name;

	//Associates a name with a texture format.
	std::unordered_map<std::string, Core::TextureFormat*> name_to_texture_format;
	std::unordered_map<Core::TextureFormat*, std::string> texture_format_to_name;

	std::string texture_path;
	TextureTemplateManagerPtr template_manager;
	TextureFormatTemplateManagerPtr format_template_manager;
};

} // namespace Framework
} // namespace GM
