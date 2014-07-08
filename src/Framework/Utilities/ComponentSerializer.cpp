
#include <GM/Framework/Utilities/ComponentSerializer.h>

using namespace GM;
using namespace Framework;

void ComponentSerializer::create_and_add_component(const EntityPtr &owner, const std::string &type, const std::string &name)
{
	sig_create_component(owner, type, name);
}
