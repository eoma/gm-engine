#pragma once

#include "TextureFormat.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace GM {
namespace Core {

class TextureFactory
{
public:
	typedef std::function<std::shared_ptr<const std::vector<unsigned char>> (const std::string &file_name, unsigned int *width, unsigned int *height, unsigned int *num_channels)> FileFetcherFunction;

	/** Creates and loads a texture as the format requires.
	 * You _must_ specify a file fetcher. The function do not care how the image is produced, as long as it is produced.
	 *
	 * The file fetcher can be whatever, eg.
	 * [](const std::string &name, unsigned int *w, unsigned int *h, unsigned int *c) -> std::shared_ptr<vector<unsigned char>> {
	 * 	SOILTextureIO io; 
	 * 	auto img = io.load(name); 
	 * 	*w = img.get_width(); 
	 * 	*h = img.get_height(); 
	 * 	*c = img.get_num_channels; 
	 * 	return tmg.get_dataptr(); 
	 * };
	 *
	 * @return A texture id
	 */
	static unsigned int create(const TextureFormat &format, FileFetcherFunction file_fetcher);
};

} // namespace Core
} // namespace GM
