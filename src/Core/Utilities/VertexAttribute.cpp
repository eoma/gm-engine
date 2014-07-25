#include "GM/Core/Utilities/VertexAttribute.h"

namespace GM {
namespace Core {

VertexAttribute::VertexAttribute(unsigned int index, unsigned int num_of_type, unsigned int type, unsigned int type_size, bool normalized, unsigned int repeat)
: index(index)
, num_of_type(num_of_type)
, type(type)
, type_size(type_size)
, normalized(normalized)
, repeat(repeat)
{
}

} // namespace Core
} // namespace GM
