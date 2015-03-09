#pragma once

namespace GM {
namespace Core {

struct DispatchIndirectCommand
{
	unsigned int num_groups_x; // How many groups in x direction
	unsigned int num_groups_y; // How many groups in y direction
	unsigned int num_groups_z; // ...
};

class DispatchCommand
{
public:
	DispatchCommand(unsigned int num_groups_x = 1, unsigned int num_groups_y = 1, unsigned int num_groups_z = 1);

	operator DispatchIndirectCommand() const;

public:
	unsigned int num_groups_x;
	unsigned int num_groups_y;
	unsigned int num_groups_z;
};

} // namespace Core
} // namespace GM
