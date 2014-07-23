#pragma once

#include <memory>

namespace GM {
namespace Core {

class BufferObject; typedef std::shared_ptr<BufferObject> BufferObjectPtr;

struct BufferAllocation
{
	BufferObjectPtr buffer;
	unsigned int allocated_size;
	unsigned int offset;
};

} // namespace Core
} // namespace GM
