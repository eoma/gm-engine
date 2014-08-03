#pragma once

#include "BufferAllocation.h"

#include <GL/gl3w.h>

namespace GM {
namespace Core {

struct RenderElementsIndirectCommand
{
	unsigned int count; // How many elements should rendered, or how many indices should we read
	unsigned int instance_count; // how many instances of the should be drawn?
	unsigned int first_index; // where in the buffer bound as IBO (GL_ELEMENT_ARRAY_BUFFER) do we find the indices? Dependent on index type (eg. byte or int)

	// Only relevant if gl version >= 4.0
	unsigned int base_vertex; // at what vertex index should we pull data from? this index is added to the index pulled from the IBO.
	unsigned int base_instance; // same functionality as base vertex. Set to 0 (zero) if gl version < 4.2.
};

struct RenderArraysIndirectCommand
{
	unsigned int count; // How many elements should rendered, or how many indices should we read
	unsigned int instance_count; // how many instances of should be drawn?
	unsigned int first; // The first vertex index to start drawing from
	unsigned int base_instance; // Set to 0 if gl version < 4.2
};

class RenderCommand
{
public:
	RenderCommand();

	// Not reccommended to use
	RenderCommand(bool is_indexed, unsigned int count, unsigned int instance_count, unsigned int first, unsigned int base_vertex = 0, unsigned int base_instance = 0);

	void set_draw_mode(const GLenum mode) { this->mode = mode; };

	template<class Vertex>
	void set_vertices(const BufferAllocation &vertex_buffer, const std::vector<Vertex> &vertices);

	template<class Instance>
	void set_instances(const BufferAllocation &instance_buffer, const std::vector<Instance> &instances);

	void set_indices(const BufferAllocation &index_buffer, const std::vector<unsigned char> &indices);
	void set_indices(const BufferAllocation &index_buffer, const std::vector<unsigned short> &indices);
	void set_indices(const BufferAllocation &index_buffer, const std::vector<unsigned int> &indices);

	operator RenderElementsIndirectCommand() const;
	operator RenderArraysIndirectCommand() const;

private:
	template<class IndexType>
	void set_indices(const BufferAllocation &index_buffer, const std::vector<IndexType> &indices, GLenum index_type);

public:
	// whether or not this is a RenderArrays or RenderElements command
	bool is_indexed; 

	// The draw mode, eg GL_POINTS, GL_LINE_STRIP, etc.
	GLenum mode;

	// if indexed, either GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT or GL_UNSIGNED_INTEGER
	GLenum index_type; 

	unsigned int count;
	unsigned int instance_count;
	unsigned int first;
	unsigned int base_vertex;
	unsigned int base_instance;
};

//
// Implementations
//

template<class Vertex>	
void RenderCommand::set_vertices(const BufferAllocation &vertex_buffer, const std::vector<Vertex> &vertices)
{
	if (vertex_buffer.offset % sizeof(Vertex) != 0)
	{
		throw clan::Exception("Vertex buffer's offset is not aligned with the size of the vertex object");
	}

	if (!is_indexed)
	{
		count = vertices.size();
		first = vertex_buffer.offset / sizeof(Vertex);
	} else {
		base_vertex = vertex_buffer.offset / sizeof(Vertex);
	}
}

template<class Instance>
void RenderCommand::set_instances(const BufferAllocation &instance_buffer, const std::vector<Instance> &instances)
{
	if (instance_buffer.offset % sizeof(Instance) != 0)
	{
		throw clan::Exception("Instance buffer's offset is not aligned with the size of the instance object");
	}

	instance_count = instances.size();
	base_instance = instance_buffer.offset / sizeof(Instance);
}

template<class IndexType>
void RenderCommand::set_indices(const BufferAllocation &index_buffer, const std::vector<IndexType> &indices, const GLenum index_type)
{
	if (index_buffer.offset % sizeof(IndexType) != 0)
	{
		throw clan::Exception("Index buffer's offset is not aligned with the size of the index type");
	}

	if (!is_indexed)
	{
		base_vertex = first;
	}
	count = indices.size();
	first = index_buffer.offset / sizeof(IndexType);
	this->index_type = index_type;

	is_indexed = true;
}

} // namespace Core
} // namespace GM
