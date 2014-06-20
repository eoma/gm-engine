#pragma once

#include "IRenderable.h"

#include <Totem/Component.h>

#include <memory>
#include <string>

namespace GM {
namespace Framework {

class Entity; typedef std::shared_ptr<Entity> EntityPtr; typedef std::weak_ptr<Entity> EntityWeakPtr;
class Renderable; typedef std::shared_ptr<Renderable> RenderablePtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Renderable : public IRenderable, public Totem::Component<Renderable> {
public:
	Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name = std::string());
	virtual ~Renderable();

private:
	Entity* owner;
	RenderSystemPtr render_system;
};

} // namespace Framework
} // namesoace GM
