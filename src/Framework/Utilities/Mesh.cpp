#include "GM/Framework/Utilities/Mesh.h"

namespace GM {
namespace Framework {

	Mesh::Mesh(const Core::RenderCommand &render_command, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager, const std::string &name, const glm::vec3 &bounds_pos, const glm::vec3 &bounds_size)
	: name(name)
	, render_command(render_command)
	, vao_layout(vao_layout)
	, bounds_pos(bounds_pos)
	, bounds_size(bounds_size)
	{
		vao = vao_manager->get_vao_for(vao_layout);
	}
        
	Mesh::Mesh(const Core::RenderCommand &render_command, const Core::VaoLayout &vao_layout, const VaoManagerPtr &vao_manager, const std::string &name)
	: name(name)
	, render_command(render_command)
	, vao_layout(vao_layout)
	, bounds_pos(glm::vec3(0,0,0))
	, bounds_size(glm::vec3(0,0,0))
	{
		vao = vao_manager->get_vao_for(vao_layout);
	}

	// Scale bounds of the mesh by the parsed scale vector
	void Mesh::scale_bounds(glm::vec3 &scale) {
		// keep track of the old size
		glm::vec3 new_size;

		// scale bounds_size, and store in new_size
		new_size = bounds_size*scale;

		// calculate the offset we should subtract from new_size and
		// bounds_pos to keep the current origin of the bounds.
		glm::vec3 offset = (new_size - bounds_size)/2.f;

		// subtract the offset and store new values in bounds_size and bounds_pos
		bounds_size = new_size - offset;
		bounds_pos = bounds_pos - offset;
	}

} // namespace Framework
} // namespace GM
