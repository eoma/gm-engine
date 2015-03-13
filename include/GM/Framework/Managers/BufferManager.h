#pragma once

#include "../../Core/GL/BufferObject.h"
#include "../../Core/Utilities/BufferAllocation.h"
#include "../../Core/Utilities/BufferOperations.h"

#include "../Utilities/Tools.h"

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

	/**
	 * Will create a BufferAllocation with size size and offset fitted to the alignment parameter.
	 */
	Core::BufferAllocation allocate(const unsigned int size, const unsigned int alignment, const GLenum usage = GL_DYNAMIC_DRAW, const BufferAllocationType allocation_type = SHARED_BUFFER, const GLenum buffer_type = GL_ARRAY_BUFFER);

	/**
	 * Allocates a buffer with size at least count*sizeof(DataStructure) and aligns the buffer offset
	 * to size of the DataStructure
	 */
	template <class DataStructure>
	Core::BufferAllocation allocate(const unsigned int count, const GLenum usage = GL_DYNAMIC_DRAW, const BufferAllocationType allocation_type = SHARED_BUFFER, const GLenum buffer_type = GL_ARRAY_BUFFER);

	/**
	 * Aligns and uploads a vector of DataStructure to a buffer.
	 */
	template <class DataStructure>
	Core::BufferAllocation allocate_and_upload(const std::vector<DataStructure> &data_structure, const GLenum usage = GL_DYNAMIC_DRAW, const BufferAllocationType allocation_type = SHARED_BUFFER)
	{
		return allocate_and_upload_multiple(usage, allocation_type, data_structure)[0];
	}

	template <class... DataStructures>
	std::vector<Core::BufferAllocation> allocate_and_upload_multiple(const std::vector<DataStructures>&... data_structures)
	{
		return allocate_and_upload_multiple(GL_DYNAMIC_DRAW, SHARED_BUFFER, data_structures...);
	}

	template <class... DataStructures>
	std::vector<Core::BufferAllocation> allocate_and_upload_multiple(const GLenum usage, const BufferAllocationType allocation_type, const std::vector<DataStructures>&... data_structures)
	{
		Core::BufferAllocation allocation = allocate(Core::total_size_plus_one(data_structures...), 0, usage, allocation_type);

		std::vector<Core::BufferAllocation> buffer_allocations;
		buffer_allocations.reserve(sizeof...(data_structures));

		align_and_upload(buffer_allocations, allocation.offset, allocation.buffer,
			data_structures...);

		return buffer_allocations;
	}

private:
	struct PoolData
	{
		Core::BufferObjectPtr buffer;
		unsigned int allocated;

		unsigned int get_unused_size() const { return buffer->get_size() - allocated; };

		GLenum get_usage() const { return buffer->get_usage(); }
		GLenum get_type() const { return buffer->get_primary_type(); }
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

	/**
	 * Uploads data_structure to buffer, but will align the offset according to the size of
	 * DataStructure. It will construct a BufferAllocation object with the relevant size
	 * and correct offset.
	 */
	template<class DataStructure>
	void align_and_upload(std::vector<Core::BufferAllocation> &allocations, 
		const unsigned int usable_offset, const Core::BufferObjectPtr &buffer,
		const std::vector<DataStructure>& data_structure)
	{
		Core::BufferAllocation allocation;
		allocation.buffer = buffer;
		allocation.allocated_size = Core::total_size(data_structure);
		allocation.offset = usable_offset;

		if (allocation.offset % sizeof(DataStructure) != 0)
		{
			// pad the offset in order to align the datastructure according the size of DataStructure
			allocation.offset += sizeof(DataStructure) - (allocation.offset % sizeof(DataStructure));
		}

		allocation.upload(data_structure);
		allocations.push_back(allocation);
	}

	/**
	 * Upload multiple sets of data structures to a buffer and align them according
	 * to the size of their data structure.
	 */
	template<class FirstDataStructure, class... RestDataStructures>
	void align_and_upload(std::vector<Core::BufferAllocation> &allocations,
		const unsigned int usable_offset, const Core::BufferObjectPtr &buffer,
		const std::vector<FirstDataStructure> first_data_structure,
		const std::vector<RestDataStructures>&... rest_data_structures)
	{
		align_and_upload(allocations, usable_offset, buffer, first_data_structure);
		const auto &last = allocations.back();
		align_and_upload(allocations, last.offset + last.allocated_size, buffer, rest_data_structures...);
	}

	PoolData create_pool(unsigned int size, unsigned int content_type = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW);

private:
	unsigned int default_pool_size;

	std::vector<PoolData> pools;
};

//
// Implementations
//

template <class DataStructure>
Core::BufferAllocation BufferManager::allocate(const unsigned int count, const GLenum usage, const BufferAllocationType allocation_type, const GLenum buffer_type)
{
	return allocate(sizeof(DataStructure)*count, sizeof(DataStructure), usage, allocation_type, buffer_type);
}

} // namespace Framework
} // namespace GM
