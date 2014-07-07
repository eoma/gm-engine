#include "GM/Framework/Utilities/BufferUse.h"

namespace GM {
namespace Framework {

int BufferUse::compare(const BufferUse &current, const BufferUse &other)
{
	int result = 0;

	if (result == 0) result = current.name - other.name;
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

} // namespace Framework
} // namespace GM
