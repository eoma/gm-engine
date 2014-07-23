#pragma once

#include "../../Core/Utilities/VaoLayout.h"

#include <map>


namespace GM {

namespace Core {
	class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
}

namespace Framework {

class VaoManager
{
public:
	VaoManager();
	~VaoManager();

	Core::VertexArrayObjectPtr get_vao_for(const Core::VaoLayout &layout);

private:
	Core::VertexArrayObjectPtr build_vao(const Core::VaoLayout &layout);

private:
	std::map<Core::VaoLayout, Core::VertexArrayObjectPtr> vaos;

};

} // namespace Framework
} // namespace GM
