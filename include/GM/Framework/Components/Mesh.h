#pragma once

#include "../DefinitionsComponentNames.h"
#include "../Entity.h"
#include "../Totem/Component.h"

#include <memory>

namespace GM {

namespace Framework {

class Mesh : public Component<Mesh>
{
public:
	Mesh(const EntityPtr &owner, const std::string &name = std::string());
	virtual ~Mesh() {};

	std::string get_type() const override { return get_static_type(); };

protected:
private:
	static std::string get_static_type() { return COMPONENT_MESH; };
};

} // namespace Framework
} // namespace GM
