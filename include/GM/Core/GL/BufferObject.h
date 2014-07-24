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

	template<class... DataStructure>
	void upload(unsigned int length, unsigned int offset, const std::vector<DataStructure>&... data_structure_collections)
	{
		if (current_type == 0) bind(); // may be unsafe?
		BufferOperations::upload(current_type, length, offset, data_structure_collections...);
	}

	template<class... DataStructure>
	void upload(const std::vector<DataStructure>&... data_structure_collections)
	{
		upload(size, 0, data_structure_collections...);
	}

private:
	// Primary type
	unsigned int primary_type;

	// Wht the buffer is currently bound as
	unsigned int current_type;

	unsigned int usage;

	unsigned int handle;

	unsigned int size;

};

} // namespace Core
} // namespace GM
