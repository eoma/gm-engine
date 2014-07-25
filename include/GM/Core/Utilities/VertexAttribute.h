#pragma once

namespace GM {
namespace Core {

class VertexAttribute
{
public:
	VertexAttribute(unsigned int index, unsigned int num_of_type, unsigned int type, unsigned int type_size, bool normalized, unsigned int repeat);

	unsigned int index;
	unsigned int num_of_type;
	unsigned int type;
	unsigned int type_size;
	bool normalized;

	// Useful for matrices
	unsigned int repeat;
};

} // namespace Core
} // namespace GM
