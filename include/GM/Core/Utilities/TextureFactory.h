#pragma once

#include "TextureFormat.h"

#include <functional>
#include <string>

namespace GM {
namespace Core {

class TextureFactory
{
	typedef std::function<const unsigned char * const(const std::string &, unsigned int *width, unsigned int *height, unsigned int *num_channels)> FileFetcherFunction;

	/** Creates and loads a texture as the format requires.
	 * You _must_ specify a file fetcher. The function do not care how the image is produced, as long as it is produced.
	 *
	 * The file fetcher can be whatever, eg. [](const std::string &name) -> RawImage { SOILTextureIO io; return io.load(name); };
	 *
	 * @return A texture id
	 */
	//static unsigned int make(const TextureFormat &format, std::function<RawImage(const std::string &)> file_fetcher);
	static unsigned int make(const TextureFormat &format, FileFetcherFunction file_fetcher);
};

} // namespace Core
} // namespace GM
