#pragma once

#include "../Managers/VaoManager.h"
#include <GM/Core/Utilities/VaoLayout.h>
#include <GM/Core/Utilities/RenderCommand.h>

#include <memory>

namespace GM {
namespace Core {
	class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
}
namespace Framework {

class Mesh
{
public:
	// Constructor with bounds
	Mesh(const Core::RenderCommand& render_command, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager, const std::string &name, const glm::vec3 &bounds_pos, const glm::vec3 &bounds_size);
    
	// Constructor without bounds
	Mesh(const Core::RenderCommand& render_command, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager, const std::string &name);

	virtual ~Mesh() {};

	const std::string &get_name() const { return name; }
	const Core::VertexArrayObjectPtr &get_vao() const { return vao; }
	const Core::VaoLayout &get_vao_layout() const { return vao_layout; }
	const Core::RenderCommand &get_render_command() const { return render_command; }

	// This might be dangerous, useful when updating the render command
	void set_render_command(const Core::RenderCommand &command) { render_command = command; }

	// Bounds stuff

	// Get bounds base position
	const glm::vec3 &get_bounds_position() const { return this->bounds_pos; }
	// Get bounds size
	const glm::vec3 &get_bounds_size() const { return this->bounds_size; }

	void scale_bounds(glm::vec3 &scale);

private:
	std::string name;
	Core::VertexArrayObjectPtr vao;
	Core::RenderCommand render_command;
	Core::VaoLayout vao_layout;

	// Bounds variables. bounds_pos is the lower left front corner of the bounds,
	// bounds_size is the distance from lower left front to upper right back of the bounds.
	glm::vec3 bounds_pos;
	glm::vec3 bounds_size;
};

} // namespace Framework
} // namespace GM
