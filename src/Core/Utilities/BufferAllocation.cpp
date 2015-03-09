#include "GM/Core/Utilities/BufferAllocation.h"

namespace GM {
namespace Core {

void BufferAllocation::upload(const std::function<void(void *destination, size_t size)> &upload_function)
{
	buffer->upload(allocated_size, offset, upload_function);
}

} // namespace Core
} // namespace GM
