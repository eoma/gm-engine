#include "GM/Core/Utilities/TextureFormat.h"

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
	if (result == 0) result = (current.parameters < other.parameters) ? -1 : (other.parameters < current.parameters ? 1 : 0);

	return result;
};

bool operator< (const TextureFormat &current, const TextureFormat &other)
{
	return TextureFormat::compare(current, other) ? true : false;
}

} // namespace Core
} // namespace GM
