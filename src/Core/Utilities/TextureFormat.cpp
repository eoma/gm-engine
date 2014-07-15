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

TextureFormat TextureFormat::create_texture2d_format(const std::string &name, bool generate_mipmap, unsigned int wrap_mode)
{
	// FIXME: Uncomment when GL comes
	TextureFormat format(/*GL_TEXTURE_2D*/ 1);

	format.add_image_file(name);

	// FIXME: uncomment when GL comes
	//format.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//format.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//format.set_parameter(GL_TEXTURE_WRAP_S, wrap_mode);
	//format.set_parameter(GL_TEXTURE_WRAP_T, wrap_mode);

	format.set_generate_mipmap(generate_mipmap);

	return format;
}

} // namespace Core
} // namespace GM
