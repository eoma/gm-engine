#pragma once

#include <vector>

namespace GM {
namespace Framework {

class BufferManager {

public:

	enum BufferAllocationType {
		SHARED_BUFFER, // Mainly used for general buffer usage like VBO, IBO, ...
		UNIQUE_BUFFER // Mainly used where you cannot share it. Relevant when you want TFB and such
	};

	struct BufferAllocation
	{
		unsigned int name;
		unsigned int allocated_size;
		unsigned int offset;
	};

	BufferManager(unsigned int default_pool_size = 32 * (1<<20));
	~BufferManager();

	BufferAllocation allocate(const unsigned int size, const BufferAllocationType type = SHARED_BUFFER);

private:
	struct PoolData
	{
		unsigned int name;
		unsigned int total_size;
		unsigned int allocated;
		unsigned int content_type; // 
		unsigned int use_type; // draw, read or copy; static, dynamic or stream

		unsigned int get_unused_size() const { return total_size - allocated; };

		bool has_space_for(const unsigned int size) const { return size <= get_unused_size(); };

		// Increases allocated by size and returns previous allocated as offset
		unsigned int allocate(const unsigned int size)
		{
			unsigned int offset = allocated;

			allocated += size;

			return offset;
		}

		PoolData()
		: name(0)
		, total_size(0)
		, allocated(0)
		, content_type(0)
		, use_type(0)
		{
		}
	};

//FIXME: Remove when gl is introduced
#ifndef GL_ARRAY_BUFFER
#	define GL_ARRAY_BUFFER 1
#endif
#ifndef GL_STATIC_DRAW
#	define GL_STATIC_DRAW 1
#endif


private:

	PoolData create_pool(unsigned int size, unsigned int content_type = GL_ARRAY_BUFFER, unsigned int use_type = GL_STATIC_DRAW);

private:
	unsigned int default_pool_size;

	std::vector<PoolData> pools;
};

} // namespace Framework
} // namespace GM
