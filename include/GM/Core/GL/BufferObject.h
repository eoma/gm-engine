#pragma once

namespace GM {
namespace Core {

// FIXME: When GL comes...
class BufferObject
{
public:
	BufferObject(const unsigned int primary_type = /*GL_ARRAY_BUFFER*/ 1);
	~BufferObject();

	// Bind as primary type
	void bind();

	// Bind as custom type
	void bind_as(const unsigned int other_type);

	void unbind();

	// This helps to keep record of what the originally was meant for
	unsigned int get_primary_type() const { return primary_type; }

	// What is the buffer currently bound as?
	unsigned int get_current_type() const { return primary_type; }

	unsigned int get_handle() const { return handle; }

private:
	// Primary type
	unsigned int primary_type;

	// Wht the buffer is currently bound as
	unsigned int current_type;

	unsigned int handle;

};

} // namespace Core
} // namespace GM
