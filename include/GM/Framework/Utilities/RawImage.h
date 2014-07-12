#pragma once

#include <vector>

class RawImage {
public:
	RawImage() : data(), width(0), height(0), num_channels(0) {};
	
	RawImage(const std::vector<unsigned char> &decoded_data,
		const unsigned int width, const unsigned int height, 
		const unsigned int num_channels)
	: data(decoded_data)
	, width(width)
	, height(height)
	, num_channels(num_channels)
	{
	}

	RawImage(const unsigned char * decoded_data,
		const unsigned int width, const unsigned int height,
		const unsigned int num_channels)
	: data(decoded_data, decoded_data + (width*height*num_channels))
	, width(width)
	, height(height)
	, num_channels(num_channels)
	{
	}

	const std::vector<unsigned char> &get_data() const { return data; };

	unsigned int get_width() const { return width; };
	unsigned int get_height() const { return height; };
	unsigned int get_num_channels() const { return num_channels; };

private:
	std::vector<unsigned char> data;
	unsigned int width;
	unsigned int height;
	unsigned int num_channels;
};
