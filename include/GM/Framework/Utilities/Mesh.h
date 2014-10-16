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
	Mesh(const Core::RenderCommand& render_command, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager, const std::string &name);
	virtual ~Mesh() {};

	const std::string &get_name() const { return name; }
	const Core::VertexArrayObjectPtr &get_vao() const { return vao; }
	const Core::VaoLayout &get_vao_layout() const { return vao_layout; }
	const Core::RenderCommand &get_render_command() const { return render_command; }

	// This might be dangerous, useful when updating the render command
	void set_render_command(const Core::RenderCommand &command) { render_command = command; }

private:
	std::string name;
	Core::VertexArrayObjectPtr vao;
	Core::RenderCommand render_command;
	Core::VaoLayout vao_layout;
};

} // namespace Framework
} // namespace GM
