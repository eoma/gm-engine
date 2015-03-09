#include "GM/Core/Utilities/BufferOperations.h"

namespace GM {
namespace Core {

void BufferOperations::upload_unsafe(const GLenum target,
	const std::function<void(void *destination, size_t size)> &upload_function,
	GLsizeiptr length, GLintptr offset)
{
	// Assume the buffer is bound

	void *destination = glMapBufferRange(target, offset, length,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

	if (destination == nullptr) {
		throw clan::Exception("Unable to map buffer range!");
	}


	upload_function(destination, length);

	glUnmapBuffer(target);
}

} // namespace Core
} // namespace GM
