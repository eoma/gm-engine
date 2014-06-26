#pragma once

#include <glm/glm.hpp>

#include <memory>

namespace GM {
namespace Framework {

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

	virtual void render() = 0;
};

} // namespace Framework
} // namespace GM
