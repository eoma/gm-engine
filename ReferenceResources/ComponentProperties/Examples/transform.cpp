#include "precomp.h"
#include "transform.h"
#include "definitions_property_names.h"

using namespace Totem;
using namespace clan;

Transform::Transform(ServerGameObjectPtr owner, const std::string &name)
: Component<Transform>(name)
{
    position_property = owner->add<Vec3f>(PROPERTY_POSITION, Vec3f(0,0,0));

	owner->replicate_property(position_property);
}
