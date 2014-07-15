#include "GM/Core/Utilities/TextureFormat.h"

#include <algorithm>

namespace GM {
namespace Core {

TextureFormat::TextureFormat(const unsigned int type)
: type(type)
, make_mipmap(false)
, image_files()
{
};

int TextureFormat::compare(const TextureFormat &current, const TextureFormat &other)
{
	int result = current.type - other.type;

	if (result == 0) result = current.make_mipmap - other.make_mipmap;
	if (result == 0) result = (current.image_files < other.image_files) ? -1 : (other.image_files < current.image_files ? 1 : 0);
	if (result == 0)
	{
		// test if current.parameters < other.parameters
		bool less_than = std::lexicographical_compare(
			current.parameters.begin(), current.parameters.end(),
			other.parameters.begin(), other.parameters.end(),
			std::owner_less<ITextureParameterPtr>());

		if (less_than)
		{
			result = -1;
		}
		else
		{
			// test if other.parameters < other.parameters
			less_than = std::lexicographical_compare(
				other.parameters.begin(), other.parameters.end(),
				current.parameters.begin(), current.parameters.end(),
				std::owner_less<ITextureParameterPtr>());

			if (less_than) result = 1;
		}
	}

	return result;
};

bool operator< (const TextureFormat &current, const TextureFormat &other)
{
	return TextureFormat::compare(current, other) ? true : false;
}

} // namespace Core
} // namespace GM
