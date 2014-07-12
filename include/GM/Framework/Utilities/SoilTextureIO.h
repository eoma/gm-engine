#pragma once

#include "ITextureIO.h"

#include <SOIL2.h>

namespace GM {
namespace Framework {

class SoilTextureIO : public ITextureIO {
public:
	SoilTextureIO();
	virtual ~SoilTextureIO();

	RawImage load(const std::string &file_name) override;
	RawImage load(const std::vector<unsigned char> &undecoded_image) override;
	RawImage load(const unsigned char * const undecoded_image, const unsigned int size) override;

	bool save(const std::string &file_name, const RawImage& image) override;
};

} // namespace Framework
} // namespace GM
