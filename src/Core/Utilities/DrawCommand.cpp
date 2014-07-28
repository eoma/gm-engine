#include "GM/Core/Utilities/DrawCommand.h"

namespace GM {
namespace Core {

DrawCommand::DrawCommand(bool is_indexed, unsigned int count, unsigned int instance_count, unsigned int first, unsigned int base_vertex, unsigned int base_instance)
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

DrawCommand::operator DrawElementsIndirectCommand() const
{
	// maybe perform a check that the command is indeed a DrawElements command?

	DrawElementsIndirectCommand command;
	command.count = count;
	command.instance_count = instance_count;
	command.first_index = first;
	command.base_vertex = base_vertex;
	command.base_instance = base_instance;

	return command;
}

DrawCommand::operator DrawArraysIndirectCommand() const
{
	// maybe perform a check that the command is indeed a DrawArrays command?

	DrawArraysIndirectCommand command;
	command.count = count;
	command.instance_count = instance_count;
	command.first = first;
	command.base_instance = base_instance;

	return command;
}

} // namespace Core
} // namespace GM
