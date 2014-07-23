#include "GM/Core/Utilities/BufferUse.h"

namespace GM {
namespace Core {

int BufferUse::compare(const BufferUse &current, const BufferUse &other)
{
	int result = 0;

	if (result == 0) result = (current.buffer < other.buffer ? -1 : (other.buffer < current.buffer ? 1 : 0));
	if (result == 0) result = current.type - other.type;

	return result;
}

bool operator< (const BufferUse &current, const BufferUse &other)
{
	return BufferUse::compare(current, other) < 0;
}

bool operator== (const BufferUse &current, const BufferUse &other)
{
	return BufferUse::compare(current, other) == 0;
}

} // namespace Core
} // namespace GM
