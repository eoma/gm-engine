#pragma once

#include "../Utilities/RawImage.h"

#include <string>
#include <vector>

namespace GM {
namespace Framework {

class ITextureIO {
public:
	virtual ~ITextureIO() {};

	/**
	 * Load image from file.
	 * Should basically load a file and call ITextureIO::load(pointer to start, size))
	 */
	virtual RawImage load(const std::string &file_name) = 0;

	/**
	 * Load undecoded image from memory, assumes vector of unsigned chars.
	 * Basically calls load(undecoded_image.data(), undecoded_data.size()).
	 */
	virtual RawImage load(const std::vector<unsigned char> &undecoded_image) = 0;

	/**
	 * Load undecoded image from memory, assumes array of unsigned char
	 */
	virtual RawImage load(const unsigned char * const undecoded_image, const unsigned int size) = 0;

	/**
	 * Save image to file. The file name must end with a valid 
	 * image extensions. It is not guaranteed that the implementation
	 * will support the given image type. Implmentations should
	 * support bmp and png.
	 *
	 * @return true if image successfully saved
	 */
	virtual bool save(const std::string &file_name, const RawImage &image) = 0;
};

} // namespace Framework
} // namespace GM
