#pragma once

#include "../Utilities/BufferOperations.h"

#include "GL/gl3w.h"

#include <memory>
#include <vector>

namespace GM {
namespace Core {

class BufferObject; typedef std::shared_ptr<BufferObject> BufferObjectPtr;

class BufferObject
{
public:
	BufferObject(const unsigned int size, const unsigned int primary_type = GL_ARRAY_BUFFER, unsigned int usage = GL_STATIC_DRAW);
	~BufferObject();

	// Bind as primary type
	void bind();

	// Bind as custom type
	void bind_as(const unsigned int other_type);

	void unbind();

	// This helps to keep record of what the originally was meant for
	unsigned int get_primary_type() const { return primary_type; }

	// What is the buffer currently bound as?
	unsigned int get_current_type() const { return current_type; }

	unsigned int get_usage() const { return usage; }

	unsigned int get_handle() const { return handle; }

	unsigned int get_size() const { return size; }

	/**
	 * Will map the buffer and call upload_function with proper destination pointer and
	 * allowed size to write.
	 */
	void upload(unsigned int length, unsigned int offset, const std::function<void(void *destination, size_t size)> &upload_function);

	/**
	 * Same as upload(size, 0, upload_function);
	 */
	void upload(const std::function<void(void *destination, size_t size)> &upload_function);

	/**
	 * Upload data_structure_collection to this buffer at offset offset and maximum size length.
	 *
	 * @param length size of the buffer to write
	 */
	template<class... DataStructure>
	void upload(unsigned int length, unsigned int offset, const std::vector<DataStructure>&... data_structure_collections)
	{
		if (current_type == 0) bind(); // may be unsafe?
		BufferOperations::upload(current_type, length, offset, data_structure_collections...);
	}

	/**
	 * Upload data_structure_collections to this buffer.
	 */
	template<class... DataStructure>
	void upload(const std::vector<DataStructure>&... data_structure_collections)
	{
		upload(size, 0, data_structure_collections...);
	}

private:
	// Primary type
	unsigned int primary_type;

	// What the buffer is currently bound as
	unsigned int current_type;

	unsigned int usage;

	unsigned int handle;

	unsigned int size;

};

} // namespace Core
} // namespace GM
