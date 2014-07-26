#pragma once

#include "VaoLayout.h"

#include <memory>

namespace GM {
namespace Core {

class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;

class VaoFactory
{
public:
	static VertexArrayObjectPtr create(const VaoLayout &vao_layout);

};

} // namespace Core
} // namespace GM
