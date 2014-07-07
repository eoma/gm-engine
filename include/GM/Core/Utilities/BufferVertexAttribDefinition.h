#pragma once

// It is assumed the buffer can act as a GL_ARRAY_BUFFER
struct BufferVertexAttribDefinition
{
	unsigned int buffer_name;
	unsigned int index;
	unsigned int size_per_index;
	unsigned int data_type;
	bool normalized;
	unsigned int stride;
	unsigned int offset;
	unsigned int divisor;

	BufferVertexAttribDefinition() : buffer_name(0), index(0), size_per_index(0), data_type(0), normalized(false), stride(0), offset(0), divisor(0) {};

	static int compare(const BufferVertexAttribDefinition &current, const BufferVertexAttribDefinition &other);
};

bool operator< (const BufferVertexAttribDefinition &current, const BufferVertexAttribDefinition &other);
