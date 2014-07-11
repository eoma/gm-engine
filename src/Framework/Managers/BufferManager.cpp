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
	for (const PoolData &pool : pools) {
		// TODO: Deallocate storage
	}
}

BufferManager::BufferAllocation BufferManager::allocate(const unsigned int size, const BufferAllocationType type)
{
	BufferAllocation allocation;
	std::vector<PoolData>::iterator iter;

	if (type == SHARED_BUFFER) // we have the freedom to allocate where it seems fit
	{
		// Locate a a pool with enough vacant size
		iter = std::find_if(pools.begin(), pools.end(), 
			[size](const PoolData &pool) { return pool.has_space_for(size); }
		);

		if (iter == pools.end()) // Must allocate new pool
		{
			unsigned int pool_size = default_pool_size;
			if (size > default_pool_size) pool_size = size;

			PoolData pool = create_pool(pool_size);

			pools.push_back(pool);
			iter = --(pools.end());
		}
	}
	else // Unique buffer
	{
		PoolData pool = create_pool(size);

		pools.push_back(pool);
		iter = --(pools.end());
	}

	PoolData &pool = *iter;

	allocation.name = pool.name;
	allocation.allocated_size = size;
	allocation.offset = pool.allocate(size);

	return allocation;
}

BufferManager::PoolData BufferManager::create_pool(unsigned int size, unsigned int content_type, unsigned int use_type)
{
	PoolData pool;

	// Generate buffer, Core::make_buffer?
	pool.name = 1;

	pool.total_size = size;
	pool.content_type = content_type;
	pool.use_type = use_type;

	//
	// TODO: Do something to allocate storage
	//

	return pool;
}

} // namespace Framework
} // namespace GM
