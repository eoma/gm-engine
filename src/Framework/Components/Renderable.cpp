#include "GM/Framework/Components/Renderable.h"

#include "GM/Framework/Systems/RenderSystem.h"

using namespace GM::Framework;

Renderable::Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name)
: Component<Renderable>(owner, name)
, render_system(render_system)
{
	render_system->add_renderable(this);
}

Renderable::~Renderable() {
	render_system->remove_renderable(this);
}
