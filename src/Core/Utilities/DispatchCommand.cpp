#include "GM/Core/Utilities/DispatchCommand.h"

namespace GM {
namespace Core {

DispatchCommand::DispatchCommand(unsigned int num_groups_x, unsigned int num_groups_y, unsigned int num_groups_z)
: num_groups_x(num_groups_x)
, num_groups_y(num_groups_y)
, num_groups_z(num_groups_z)
{
}

DispatchCommand::operator DispatchIndirectCommand() const
{
	DispatchIndirectCommand indirect;

	indirect.num_groups_x = num_groups_x;
	indirect.num_groups_y = num_groups_y;
	indirect.num_groups_z = num_groups_z;

	return indirect;
}

} // namespace Core
} // namespace GM
