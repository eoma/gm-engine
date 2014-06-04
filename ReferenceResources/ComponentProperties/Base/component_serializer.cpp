#include "precomp.h"
#include "component_serializer.h"

void ComponentSerializer::create_and_add_component(const ServerGameObjectPtr &owner, const std::string &type, const std::string &name)
{
	sig_create_component.invoke(owner, type, name);
}
