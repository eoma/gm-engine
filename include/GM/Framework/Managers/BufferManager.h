#pragma once

#include "../../Core/GL/BufferObject.h"
#include "../../Core/Utilities/BufferAllocation.h"

#include <GL/gl3w.h>

#include <vector>

namespace GM {
namespace Framework {

class BufferManager {

public:

	enum BufferAllocationType {
		SHARED_BUFFER, // Mainly used for general buffer usage like VBO, IBO, ...
		UNIQUE_BUFFER // Mainly used where you cannot share it. Relevant when you want TFB and such
	};

	BufferManager(unsigned int default_pool_size = 32 * (1<<20));
	~BufferManager();

	Core::BufferAllocation allocate(const unsigned int size, const BufferAllocationType type = SHARED_BUFFER);

private:
	struct PoolData
	{
		Core::BufferObjectPtr buffer;
		unsigned int allocated;

		unsigned int get_unused_size() const { return buffer->get_size() - allocated; };

		bool has_space_for(const unsigned int size) const { return size <= get_unused_size(); };

		// Increases allocated by size and returns previous allocated as offset
		unsigned int allocate(const unsigned int size)
		{
			unsigned int offset = allocated;

			allocated += size;

			return offset;
		}

		PoolData(const Core::BufferObjectPtr &buffer)
		: buffer(buffer)
		, allocated(0)
		{
			// Test if buffer is nullptr?
		}
	};

private:

	PoolData create_pool(unsigned int size, unsigned int content_type = GL_ARRAY_BUFFER, unsigned int use_type = GL_STATIC_DRAW);

private:
	unsigned int default_pool_size;

	std::vector<PoolData> pools;
};

} // namespace Framework
} // namespace GM
