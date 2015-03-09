#pragma once

#include "Tools.h"

#include <ClanLib/core.h>

#include <GL/gl3w.h>

#include <functional>
#include <memory>

namespace GM {
namespace Core {

class BufferOperations
{
public:
	static void upload_unsafe(const GLenum target,
		const std::function<void(void *destination, size_t size)> &upload_function,
		GLsizeiptr length, GLintptr offset = 0);

	template<class... DataStructures>
	static void upload(const GLenum target,	GLsizeiptr length, GLintptr offset,
		const std::vector<DataStructures>&... data_structures)
	{
		GLsizeiptr data_size = total_size(data_structures...);

		if (data_size > length) {
			throw clan::Exception(clan::string_format("Total size of data structures (%1) is bigger than requested upload size (%2).", (long long)data_size, (long long)length));
		}

		auto upload_function = [data_structures...] (void *destination, size_t /*mapped_size*/) {
			copy_func(destination, data_structures...);
		};

		upload_unsafe(target, upload_function, data_size, offset);
	}

private:

	template <class Single>
	static void copy_func(void *destination, const std::vector<Single>& single);

	template <class Head, class... Tail>
	static void copy_func(void *destination, const Head& head, const Tail&... tail);
};

//
// Implementations
//

template <class Single>
void BufferOperations::copy_func(void *destination, const std::vector<Single>& single)
{
	Single* destination_with_type = reinterpret_cast<Single*>(destination);
	std::copy(single.begin(), single.end(), destination_with_type);
}

template <class Head, class... Tail>
void BufferOperations::copy_func(void *destination, const Head& head, const Tail&... tail)
{
	copy_func(destination, head);
	copy_func(destination + total_size(head), tail...);
}

} // namespace Core
} // namespace GM
