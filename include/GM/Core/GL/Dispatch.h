#pragma once

#include "../Utilities/DispatchCommand.h"

namespace GM {
namespace Core {

class Dispatch
{
public:
	static void dispatch(const DispatchCommand &command);
};

} // namespace Core
} // namespace GM
