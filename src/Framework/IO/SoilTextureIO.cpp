#include "GM/Framework/IO/SoilTextureIO.h"
#include <SOIL2.h>
#include <ClanLib/core.h>

#include <cctype>
#include <algorithm>

namespace GM {
namespace Framework {

SoilTextureIO::SoilTextureIO()
{
}

SoilTextureIO::~SoilTextureIO()
{
}

RawImage SoilTextureIO::load(const std::string &file_name)
{
	clan::DataBuffer buffer = clan::File::read_bytes(file_name);
	return load(buffer.get_data<unsigned char>(), buffer.get_size());
}

RawImage SoilTextureIO::load(const std::vector<unsigned char> &undecoded_image)
{
	return load(undecoded_image.data(), undecoded_image.size());
}

RawImage SoilTextureIO::load(const unsigned char * const undecoded_image, const unsigned int size)
{
	int width;
	int height;
	int num_channels;
	unsigned char * decoded_data = SOIL_load_image_from_memory(undecoded_image, size, &width, &height, &num_channels, SOIL_LOAD_AUTO);

	if (decoded_data == nullptr)
	{
		throw clan::Exception("Unable to decode image!");
	}

	RawImage image(decoded_data, width, height, num_channels);

	SOIL_free_image_data(decoded_data);

	return image;
}

bool SoilTextureIO::save(const std::string &file_name, const RawImage &image)
{
	bool successful = false;

	int image_type = -1;

	// Determine image type based in file extension
	size_t pos = file_name.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string ext = file_name.substr(pos+1);
		// Perform simple character lower case conversion
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext == "tga")
		{
			image_type = SOIL_SAVE_TYPE_TGA;
		}
		else if (ext == "bmp")
		{
			image_type = SOIL_SAVE_TYPE_BMP;
		}
		else if (ext == "png")
		{
			image_type = SOIL_SAVE_TYPE_PNG;
		}
	}

	successful = SOIL_save_image(file_name.c_str(), image_type, image.get_width(), image.get_height(), image.get_num_channels(), image.get_data().data());

	return successful;
}

} // namespace Framework
} // namespace GM
