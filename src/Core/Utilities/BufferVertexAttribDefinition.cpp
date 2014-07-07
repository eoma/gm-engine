#include "GM/Core/Utilities/BufferVertexAttribDefinition.h"

namespace GM {
namespace Core {

int BufferVertexAttribDefinition::compare(const BufferVertexAttribDefinition &current, const BufferVertexAttribDefinition &other)
{
	int result = 0;

	if (result == 0) result = current.buffer_name - other.buffer_name;
	if (result == 0) result = current.index - other.index;
	if (result == 0) result = current.size_per_index - other.size_per_index;
	if (result == 0) result = current.data_type - other.data_type;
	if (result == 0) result = current.normalized - other.normalized;
	if (result == 0) result = current.stride - other.stride;
	if (result == 0) result = current.offset - other.offset;
	if (result == 0) result = current.divisor - other.divisor;

	return result;
};

bool operator< (const BufferVertexAttribDefinition &current, const BufferVertexAttribDefinition &other) {
	return BufferVertexAttribDefinition::compare(current, other) < 0;
}

} // namespace Core
} // namespace GM
