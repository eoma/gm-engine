#pragma once

#include "../RenderLayers.h"

#include <glm/glm.hpp>

#include <memory>

namespace GM {
namespace Framework {

class Camera;
class Material; typedef std::shared_ptr<Material> MaterialPtr;
class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;
class IRenderable; typedef std::shared_ptr<IRenderable> IRenderablePtr;

class IRenderable {
public:
	virtual ~IRenderable() {};

	virtual glm::mat4 get_world_matrix() const = 0;
	virtual glm::mat4 get_object_matrix() const = 0;

	virtual bool is_culled() const = 0;

	// should test if it heas been culled or just not visible
	// eg. return (culled || !visible)
	virtual bool is_invisible() const = 0;

	// Which render layers do you want to be in?
	// The format is defined as a bitfield. If you want
	// to be in layer 0, 1 and 2, you return 1<<0 + 1<<1 + 1<<2.
	// Default should be 1.
	virtual unsigned int get_render_layers() const = 0;

	// This should not change while in a render system.
	// If you want to change it, remove yourself from the render system,
	// change it and add yourself again.
	virtual bool has_custom_render() const = 0;

	//  Will be called if has_custom_render() returns true
	virtual void custom_render(Camera *camera) = 0;

	// The following are relevant in sorting and subsequently in constructing
	// a draw command buffer

	virtual const MaterialPtr &get_material() const = 0;

	virtual const MeshPtr &get_mesh() const = 0;

	virtual void update_uniforms(Camera *camera, const std::string &render_pass_name = "standard") = 0;
};

} // namespace Framework
} // namespace GM
