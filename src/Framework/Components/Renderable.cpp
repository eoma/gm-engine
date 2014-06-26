#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Systems/RenderSystem.h"

using namespace GM::Framework;

Renderable::Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const unsigned int render_layers, const std::string &name)
: Component<Renderable>(owner, name)
, render_system(render_system)
, render_layers(render_layers)
{
	render_system->add_renderable(this);

	world_matrix_property = owner->add(PROPERTY_WORLD_MATRIX, glm::mat4());
	object_matrix_property = owner->add(PROPERTY_OBJECT_MATRIX, glm::mat4());

	culled_property = owner->add(PROPERTY_CULLED, false);
	visible_property = owner->add(PROPERTY_VISIBLE, false);
}

Renderable::~Renderable() {
	render_system->remove_renderable(this);
}

void Renderable::render() {

}
