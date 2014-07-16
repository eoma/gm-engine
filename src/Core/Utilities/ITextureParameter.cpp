#include "GM/Core/Utilities/ITextureParameter.h"

namespace GM {
namespace Core {

bool operator< (const ITextureParameter &current, const ITextureParameter &other)
{
	return ITextureParameter::compare(current, other) < 0;
}

} // namespace Core
} // namespace GM
