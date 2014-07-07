#pragma once

#include "BufferVertexAttribDefinition.h"
#include "BufferUse.h"

#include <vector>

namespace GM {
namespace Core {

class VaoLayout {
public:
	VaoLayout();
	~VaoLayout();

	VaoLayout &for_buffer(const unsigned int buffer_name);
	VaoLayout &use_as(const unsigned int buffer_type);

	// Specify vertex attributes
	VaoLayout &bind(
		const unsigned int index,
		const unsigned int size_per_index,
		const unsigned int data_type,
		const bool normalized,
		const unsigned int stride = 0,
		const unsigned int offset = 0,
		const unsigned int divisor = 0);

	// It is possible to make specialized versions of bind
	// for specific types, for example mat4, vec{2,3,4}.

	// Relevant for VaoManager.
	const std::vector<BufferVertexAttribDefinition> &get_definitions() const;
	const std::vector<BufferUse> &get_used_buffers() const;

	bool operator< (const VaoLayout &other) const;

private:
	unsigned int active_buffer;
	unsigned int active_type;

	// The definitions are sorted
	std::vector<BufferVertexAttribDefinition> definitions;

	std::vector<BufferUse> used_buffers;
};

} // namespace Core
} // namespace GM
