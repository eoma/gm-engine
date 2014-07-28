#include "GM/Framework/Utilities/Mesh.h"

namespace GM {
namespace Framework {

	Mesh::Mesh(const std::string &name, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager)
		: name(name)
{
		vao = vao_manager->get_vao_for(vao_layout);
}

} // namespace Framework
} // namespace GM
