#pragma once

#include "../GL/BufferObject.h"
#include "BufferAllocation.h"

#include "../../Framework/Utilities/Tools.h"

#include <GL/gl3w.h>

#include <functional>
#include <memory>
#include <sstream>

namespace GM {
namespace Core {

class BufferOperations
{
public:
	static void upload_unsafe(const BufferObjectPtr &buffer,
		std::function<void(void *destination, size_t size)> upload_function,
		GLintptr offset = 0, GLsizeiptr size = 0);

	static void upload_unsafe(const BufferAllocation &buffer_allocation,
		std::function<void(void *destination, size_t size)> upload_function);

	template<class... DataStructures>
	static void upload(const BufferObjectPtr &buffer, GLintptr offset,
		GLsizeiptr size, const std::vector<DataStructures>&... data_structures);
	
	template<class... DataStructures>
	static void upload(const BufferObjectPtr &buffer, const std::vector<DataStructures>&... data_structures);

	template<class... DataStructures>
	static void upload(const BufferAllocation &buffer_allocation, const std::vector<DataStructures>&... data_structures);

private:

	template <class Single>
	static void copy_func(void *destination, const std::vector<Single>& single);

	template <class Head, class... Tail>
	static void copy_func(void *destination, const std::vector<Head>& head, const std::vector<Tail>&... tail);
	
};

//
// Implementations
//

template<class... DataStructures>
void BufferOperations::upload(const BufferObjectPtr &buffer, GLintptr offset, GLsizeiptr size, const std::vector<DataStructures>&... data_structures)
{
	size_t data_size = Framework::total_size(data_structures...);

	if (data_size > size)
	{
		std::stringstream ss;
		ss << "Total size of data structures (" << data_size << ") is bigger than requested upload size (" << size << ")";
		throw std::runtime_error(ss.str());
	}

	upload_unsafe(buffer, [&](void *destination, size_t mapped_size) {
		copy_func(destination, data_structures...);
	},
	offset, data_size);
}

template<class... DataStructures>
void BufferOperations::upload(const BufferObjectPtr &buffer, const std::vector<DataStructures>&... data_structures)
{
	upload(buffer, 0, buffer->get_size(), data_structures...);
}

template<class... DataStructures>
void BufferOperations::upload(const BufferAllocation &buffer_allocation, const std::vector<DataStructures>&... data_structures)
{
	upload(buffer_allocation.buffer, buffer_allocation.offset, buffer_allocation.allocated_size, data_structures...);
}

template <class Single>
void BufferOperations::copy_func(void *destination, const std::vector<Single>& single)
{
	Single* destination_with_type = reinterpret_cast<Single*>(destination);
	std::copy(single.begin(), single.end(), destination_with_type);
}

template <class Head, class... Tail>
void BufferOperations::copy_func(void *destination, const std::vector<Head>& head, const std::vector<Tail>&... tail)
{
	copy_func(destination, head);
	copy_func(destination + Framework::total_size(head), tail...);
}

} // namespace Core
} // namespace GM
