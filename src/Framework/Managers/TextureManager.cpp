#include "GM/Framework/Managers/TextureManager.h"

#include "GM/Framework/IO/ITextureIO.h"

#include <ClanLib/core.h>

namespace GM {
namespace Framework {

TextureManager::TextureManager(const ITextureIOPtr &texture_io)
: texture_io(texture_io)
, loaded_images()
{
}

const RawImage &TextureManager::load_image(const std::string &file_name)
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

} // namespace Framework
} // namespace GM
