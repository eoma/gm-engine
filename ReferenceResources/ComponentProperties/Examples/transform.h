#pragma once

#include "definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/Components/replicated_component.h>

class Transform : public Totem::Component<Transform>, public ReplicatedComponent
{
public:
	Transform(ServerGameObjectPtr owner, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_TRANSFORM; }

protected:
	Totem::Property<clan::Vec3f> position_property;
};
