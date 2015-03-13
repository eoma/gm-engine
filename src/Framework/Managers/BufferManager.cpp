#include "GM/Framework/Managers/BufferManager.h"

#include <algorithm>

namespace GM {
namespace Framework {

BufferManager::BufferManager(unsigned int default_pool_size)
: default_pool_size(default_pool_size)
{
}

BufferManager::~BufferManager()
{
}

Core::BufferAllocation BufferManager::allocate(const unsigned int size, const unsigned int alignment, const GLenum usage, const BufferAllocationType allocation_type, const GLenum buffer_type)
{
	Core::BufferAllocation allocation;
	std::vector<PoolData>::iterator iter;

	if (allocation_type == SHARED_BUFFER) // we have the freedom to allocate where it seems fit
	{
		// Locate a a pool with enough vacant size
		iter = std::find_if(pools.begin(), pools.end(), 
			[usage, size, alignment, buffer_type](const PoolData &pool) {
				return pool.get_usage() == usage &&
				       pool.get_type() == buffer_type &&
				       pool.has_space_for(size + alignment);
			}
		);

		if (iter == pools.end()) // Must allocate new pool
		{
			unsigned int pool_size = default_pool_size;
			if (size > default_pool_size) pool_size = size;

			PoolData pool = create_pool(pool_size, buffer_type, usage);

			pools.push_back(pool);
			iter = --(pools.end());
		}
	}
	else // Unique buffer
	{
		PoolData pool = create_pool(size, buffer_type, usage);

		pools.push_back(pool);
		iter = --(pools.end());
	}

	PoolData &pool = *iter;

	allocation.buffer = pool.buffer;
	allocation.allocated_size = size;

	if (alignment != 0 && (pool.allocated % alignment) != 0) {
		unsigned int padding = alignment - (pool.allocated % alignment);
		allocation.offset = pool.allocate(size + padding);
		allocation.offset += padding;
	} else {
		allocation.offset = pool.allocate(size);
	}

	return allocation;
}

BufferManager::PoolData BufferManager::create_pool(unsigned int size, unsigned int content_type, unsigned int usage)
{
	return PoolData(std::make_shared<Core::BufferObject>(size, content_type, usage));
}

} // namespace Framework
} // namespace GM
