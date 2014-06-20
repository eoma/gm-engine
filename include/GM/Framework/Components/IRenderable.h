#pragma once

#include <glm/glm.hpp>

#include <memory>

namespace GM {
namespace Framework {

class IRenderable; typedef std::shared_ptr<IRenderable> IRenderablePtr;

class IRenderable {
public:
	virtual ~IRenderable() = 0;

	virtual glm::mat4 get_world_matrix() = 0;
	virtual glm::mat4 get_object_matrix() = 0;

	virtual bool is_culled() = 0;

	// should test if it heas been culled or just not visible
	// eg. return (culled || !visible)
	virtual bool is_invisible() = 0;
};

} // namespace Framework
} // namespace GM
