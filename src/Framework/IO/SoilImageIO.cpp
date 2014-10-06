#include "GM/Framework/IO/SoilImageIO.h"
#include <SOIL2.h>
#include <ClanLib/core.h>

#include <cctype>
#include <algorithm>

#include "lodepng.h"

namespace GM {
namespace Framework {

SoilImageIO::SoilImageIO()
{
}

SoilImageIO::~SoilImageIO()
{
}

RawImage SoilImageIO::load(const std::string &file_name)
{
	//std::cout << "Will load image file " << file_name << std::endl;
	clan::DataBuffer buffer = clan::File::read_bytes(file_name);
	return load(buffer.get_data<unsigned char>(), buffer.get_size());
}

RawImage SoilImageIO::load(const std::vector<unsigned char> &undecoded_image)
{
	return load(undecoded_image.data(), undecoded_image.size());
}

RawImage SoilImageIO::load(const unsigned char * const undecoded_image, const unsigned int size)
{

	if (is_png(undecoded_image, size))
	{
		return load_via_LodePNG(undecoded_image, size);
	}
	else
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
}

bool SoilImageIO::save(const std::string &file_name, const RawImage &image)
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

/**
 * Inspect the assumed png's header to confirm it is a png
 */
bool SoilImageIO::is_png(const unsigned char *undecoded_image, const unsigned int size)
{

	LodePNGState state;
	unsigned int w = 0, h = 0, error = 0;

	lodepng_state_init(&state);

	error = lodepng_inspect(&w, &h, &state, undecoded_image, size);

	lodepng_state_cleanup(&state);

	//if (error)
	//{
	//	std::cout << clan::string_format("is_png test: Unable to decode image, %1", lodepng_error_text(error)) << std::endl;
	//}

	return (error == 0); // No errors means we can decode via LodePNG
}

/**
 * Load image data as png, as LodePNG is better at decoding png than
 * SOIL
 */
RawImage SoilImageIO::load_via_LodePNG(const unsigned char *undecoded_image, const unsigned int size)
{
		unsigned int width = 0, height = 0, num_channels = 0;
		unsigned int error = 0;

		std::vector<unsigned char> decoded_image;

		lodepng::State png_state;
		// if you want to toy with the output bitdepth and num channels
		// uncomment this snippet and modify values in png_state.info_raw
		// based on data in png_state.info_png
		//lodepng_inspect(&width, &height, &png_state, undecoded_image, size); 

		error = lodepng::decode(decoded_image, width, height, png_state,
		                        undecoded_image, size);

		if (error)
		{
			throw clan::Exception(clan::string_format("Unable to decode image, %1", lodepng_error_text(error)));
		}

		num_channels = lodepng_get_channels(&(png_state.info_raw));

		return RawImage(decoded_image, width, height, num_channels);
}

} // namespace Framework
} // namespace GM
