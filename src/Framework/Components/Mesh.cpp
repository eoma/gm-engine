#include "GM/Framework/Components/Mesh.h"

namespace GM {
namespace Framework {

	Mesh::Mesh(const EntityPtr &owner, const std::string &name)
		: Component<Mesh>(owner, name)
{
}

} // namespace Framework
} // namespace GM
