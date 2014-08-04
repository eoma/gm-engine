#include "GM/Core/Utilities/RenderCommand.h"

namespace GM {
namespace Core {

RenderCommand::RenderCommand()
: is_indexed(false)
, mode(GL_TRIANGLE_STRIP)
, index_type(GL_UNSIGNED_INT)
, count(0)
, instance_count(1)
, first(0)
, base_vertex(0)
, base_instance(0)
{

}

RenderCommand::RenderCommand(bool is_indexed, unsigned int count, unsigned int instance_count, unsigned int first, unsigned int base_vertex, unsigned int base_instance)
: is_indexed(is_indexed)
, mode(GL_TRIANGLE_STRIP)
, index_type(GL_UNSIGNED_INT)
, count(count)
, instance_count(instance_count)
, first(first)
, base_vertex(base_vertex)
, base_instance(base_instance)
{

}

RenderCommand::operator RenderElementsIndirectCommand() const
{
	// maybe perform a check that the command is indeed a RenderElements command?

	RenderElementsIndirectCommand command;
	command.count = count;
	command.instance_count = instance_count;
	command.first_index = first;
	command.base_vertex = base_vertex;
	command.base_instance = base_instance;

	return command;
}

RenderCommand::operator RenderArraysIndirectCommand() const
{
	// maybe perform a check that the command is indeed a RenderArrays command?

	RenderArraysIndirectCommand command;
	command.count = count;
	command.instance_count = instance_count;
	command.first = first;
	command.base_instance = base_instance;

	return command;
}

void RenderCommand::set_indices(const BufferAllocation &index_buffer, const std::vector<unsigned char> &indices)
{
	set_indices(index_buffer, indices, GL_UNSIGNED_BYTE);
}

void RenderCommand::set_indices(const BufferAllocation &index_buffer, const std::vector<unsigned short> &indices)
{
	set_indices(index_buffer, indices, GL_UNSIGNED_SHORT);
}

void RenderCommand::set_indices(const BufferAllocation &index_buffer, const std::vector<unsigned int> &indices)
{
	set_indices(index_buffer, indices, GL_UNSIGNED_INT);
}

void RenderCommand::set_vertices(const BufferAllocation &vertex_buffer, const unsigned int count, const unsigned int vertex_size)
{
	if (vertex_buffer.offset % vertex_size != 0)
	{
		throw clan::Exception("Vertex buffer's offset is not aligned with the size of the vertex object");
	}

	if (!is_indexed)
	{
		this->count = count;
		first = vertex_buffer.offset / vertex_size;
	} else {
		base_vertex = vertex_buffer.offset / vertex_size;
	}
}

void RenderCommand::set_instances(const BufferAllocation &instance_buffer, const unsigned int count, const unsigned int instance_size)
{
	if (instance_buffer.offset % instance_size != 0)
	{
		throw clan::Exception("Instance buffer's offset is not aligned with the size of the instance object");
	}

	instance_count = count;
	base_instance = instance_buffer.offset / instance_size;
}

} // namespace Core
} // namespace GM
