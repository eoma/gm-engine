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

typedef unsigned int TextureId;

class TextureManager
{
public:
	TextureManager(const ITextureIOPtr &texture_io);
	~TextureManager();

	const RawImage &load_image(const std::string &file_name);

	TextureId load_texture(const Core::TextureFormat &format);

private:
	ITextureIOPtr texture_io;

	std::map<std::string, RawImage> loaded_images;

	std::map<RawImage*, std::vector<TextureId>> image_to_texture_deps;

	// TODO: still unclear
	std::map<TextureId, Core::TextureFormat> texture_id_to_format;
	std::map<Core::TextureFormat, TextureId> format_to_texture_id;
};

} // namespace Framework
} // namespace GM
