#pragma once

#include "BufferVertexAttribDefinition.h"
#include "BufferUse.h"
#include "BufferAllocation.h"

#include "VertexAttribute.h"
#include "VaoArg.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace GM {
namespace Core {

class BufferObject; typedef std::shared_ptr<BufferObject> BufferObjectPtr;

class VaoLayout
{
public:
	VaoLayout();
	~VaoLayout();

	VaoLayout &for_buffer(const BufferObjectPtr &buffer);
	VaoLayout &for_buffer(const BufferAllocation &buffer);
	VaoLayout &use_as(const unsigned int buffer_type);

	// Specify vertex attributes
	VaoLayout &bind(
		const unsigned int index,
		const unsigned int num_of_type,
		const unsigned int data_type,
		const bool normalized,
		const unsigned int stride = 0,
		const unsigned int offset = 0,
		const unsigned int divisor = 0);

	template<class Data>
	VaoLayout &bind(const unsigned int index, const unsigned int stride = 0, const unsigned int offset = 0, const unsigned int divisor = 0)
	{
		VertexAttribute attrib = VaoArg<Data>(index);
		return bind(attrib.index, attrib.num_of_type, attrib.type, attrib.normalized, stride, offset, divisor);
	}


	template<class... Datas>
	VaoLayout &bind_interleaved(const VaoArg<Datas>&... interleaved_attribs)
	{
		return bind_interleaved(0, 0, {interleaved_attribs...});
	}

	template<class... Datas>
	VaoLayout &bind_interleaved(const unsigned int offset, const unsigned int divisor, const VaoArg<Datas>&... interleaved_attribs)
	{
		return bind_interleaved(offset, divisor, {interleaved_attribs...});
	}
	
	VaoLayout &bind_interleaved(const unsigned int offset, const unsigned int divisor, const std::vector<VertexAttribute> &interleaved_attribs);

	// Relevant for VaoManager.
	const std::vector<BufferVertexAttribDefinition> &get_definitions() const;
	const std::vector<BufferUse> &get_used_buffers() const;

	bool operator< (const VaoLayout &other) const;

private:
	BufferObjectPtr active_buffer;
	unsigned int active_type;

	// The definitions are sorted
	std::vector<BufferVertexAttribDefinition> definitions;

	std::vector<BufferUse> used_buffers;
};

} // namespace Core
} // namespace GM
