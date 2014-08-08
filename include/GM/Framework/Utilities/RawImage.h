#pragma once

#include <memory>
#include <vector>

namespace GM {
namespace Framework {

class RawImage; typedef std::shared_ptr<RawImage> RawImagePtr;

class RawImage {
public:
	RawImage() : dataptr(std::make_shared<std::vector<unsigned char>>()), width(0), height(0), num_channels(0) {};
	
	RawImage(const std::vector<unsigned char> &decoded_data,
		const unsigned int width, const unsigned int height, 
		const unsigned int num_channels)
	: dataptr(std::make_shared<std::vector<unsigned char>>(decoded_data))
	, width(width)
	, height(height)
	, num_channels(num_channels)
	{
	}

	RawImage(const unsigned char * decoded_data,
		const unsigned int width, const unsigned int height,
		const unsigned int num_channels)
	: dataptr(std::make_shared<std::vector<unsigned char>>(decoded_data, decoded_data + (width*height*num_channels)))
	, width(width)
	, height(height)
	, num_channels(num_channels)
	{
	}

	const std::vector<unsigned char> &get_data() const { return *dataptr; };
	const std::shared_ptr<std::vector<unsigned char>> get_dataptr() const { return dataptr; };

	unsigned int get_width() const { return width; };
	unsigned int get_height() const { return height; };
	unsigned int get_num_channels() const { return num_channels; };

private:
	std::shared_ptr<std::vector<unsigned char>> dataptr;
	unsigned int width;
	unsigned int height;
	unsigned int num_channels;
};

} // namespace Framework
} // namespace GM
