#pragma once

#include <memory>

namespace GM {
namespace Core {

class BufferObject; typedef std::shared_ptr<BufferObject> BufferObjectPtr;

// It is assumed the buffer can act as a GL_ARRAY_BUFFER
struct BufferVertexAttribDefinition
{
	BufferObjectPtr buffer;
	unsigned int index;
	unsigned int size_per_index;
	unsigned int data_type;
	bool normalized;
	unsigned int stride;
	unsigned int offset;
	unsigned int divisor;

	BufferVertexAttribDefinition() : buffer(nullptr), index(0), size_per_index(0), data_type(0), normalized(false), stride(0), offset(0), divisor(0) {};

	static int compare(const BufferVertexAttribDefinition &current, const BufferVertexAttribDefinition &other);
};

bool operator< (const BufferVertexAttribDefinition &current, const BufferVertexAttribDefinition &other);

} // namespace Core
} // namespace GM
