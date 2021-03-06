#pragma once

#include "IImageIO.h"

namespace GM {
namespace Framework {

class SoilImageIO : public IImageIO {
public:
	SoilImageIO();
	virtual ~SoilImageIO();

	RawImage load(const std::string &file_name) override;
	RawImage load(const std::vector<unsigned char> &undecoded_image) override;
	RawImage load(const unsigned char * const undecoded_image, const unsigned int size) override;

	bool save(const std::string &file_name, const RawImage& image) override;

private:
	bool is_png(const unsigned char * const undecoded_image, const unsigned int size); // if data is png we let LodePNG load it
	RawImage load_via_LodePNG(const unsigned char * const undecoded_image, const unsigned int size);
};

} // namespace Framework
} // namespace GM
