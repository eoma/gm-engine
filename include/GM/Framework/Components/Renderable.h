#pragma once

#include "IRenderable.h"
#include "../DefinitionsComponentNames.h"

#include "../Totem/Component.h"

#include <memory>
#include <string>

namespace GM {
namespace Framework {

class Renderable; typedef std::shared_ptr<Renderable> RenderablePtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Renderable : public IRenderable, public Component<Renderable> {
public:
	Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name = std::string());
	virtual ~Renderable();

	std::string get_type() const override { return get_static_type(); }

public:
	static std::string get_static_type() { return COMPONENT_RENDERABLE; }

private:
	RenderSystemPtr render_system;
};

} // namespace Framework
} // namesoace GM
