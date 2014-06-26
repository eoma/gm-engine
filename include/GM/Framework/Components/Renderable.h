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
	Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, unsigned int render_layers = 1, const std::string &name = std::string());
	virtual ~Renderable();

	std::string get_type() const override { return get_static_type(); }

	virtual	glm::mat4 get_world_matrix() const override { return world_matrix_property; };
	virtual	glm::mat4 get_object_matrix() const override { return object_matrix_property; };

	virtual bool is_culled() const override { return culled_property; };
	virtual bool is_invisible() const override { return !visible_property || is_culled(); };

	virtual unsigned int get_render_layers() const override { return render_layers; };

	virtual void render() override;
public:
	static std::string get_static_type() { return COMPONENT_RENDERABLE; }

private:
	RenderSystemPtr render_system;
	const unsigned int render_layers;

	Property<glm::mat4> world_matrix_property;
	Property<glm::mat4> object_matrix_property;

	Property<bool> culled_property;
	Property<bool> visible_property;
};

} // namespace Framework
} // namesoace GM
