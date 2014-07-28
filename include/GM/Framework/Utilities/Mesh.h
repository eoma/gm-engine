#pragma once

#include "../Managers/VaoManager.h"
#include <GM/Core/Utilities/VaoLayout.h>

#include <memory>

namespace GM {
namespace Core {
	class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
}
namespace Framework {

class Mesh
{
public:
	Mesh(const std::string &name, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager);
	virtual ~Mesh() {};

	const std::string &get_name() const { return name; }
	const Core::VertexArrayObjectPtr &get_vao() const { return vao; }

private:
	std::string name;
	Core::VertexArrayObjectPtr vao;
};

} // namespace Framework
} // namespace GM
