#include "GM/Core/GL/Dispatch.h"

#include <GL/gl3w.h>

namespace GM {
namespace Core {

void Dispatch::dispatch(const DispatchCommand &command)
{
	glDispatchCompute(command.num_groups_x, command.num_groups_y, command.num_groups_z);
}

} // namespace Core
} // namespace GM
