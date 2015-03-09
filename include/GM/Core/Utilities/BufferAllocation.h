#pragma once

#include "BufferOperations.h"
#include "../GL/BufferObject.h"

#include <memory>
#include <vector>

namespace GM {
namespace Core {

class BufferObject; typedef std::shared_ptr<BufferObject> BufferObjectPtr;

struct BufferAllocation
{
	BufferObjectPtr buffer;
	unsigned int allocated_size;
	unsigned int offset;

	void upload(const std::function<void(void *destination, size_t size)> &upload_function);

	template <class... DataStructure>
	void upload(const std::vector<DataStructure>&... data_structure_collections)
	{
		buffer->upload(allocated_size, offset, data_structure_collections...);
	}
};

} // namespace Core
} // namespace GM
