#include "GM/Framework/Primitives/QuadXZPrimitive.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/VaoManager.h"
#include "GM/Framework/Utilities/Mesh.h"
#include "GM/Framework/DefinitionsPrimitiveNames.h"

#include "GM/Core/Utilities/VaoLayout.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/RenderCommand.h"

#include <glm/glm.hpp>

#include <GL/gl3w.h>

#include <vector>
using namespace GM;
using namespace Framework;

QuadXZPrimitive::QuadXZPrimitive()
	: IPrimitive(GM_PRIMITIVE_QUAD_XZ)
{
}

MeshPtr QuadXZPrimitive::create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) {
	struct MyVertex {
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	std::vector<MyVertex> vertices{
		{ {-0.5f, 0.0f,-0.5f }, { 0, 0 } },
		{ { 0.5f, 0.0f,-0.5f }, { 1, 0 } },
		{ {-0.5f, 0.0f, 0.5f }, { 0, 1 } },
		{ { 0.5f, 0.0f, 0.5f }, { 1, 1 } }
	};

	auto vertex_allocation = buffer_manager->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(vertex_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind_interleaved(
					Core::VaoArg<glm::vec3>(Core::ShaderConstants::Position),
					Core::VaoArg<glm::vec2>(Core::ShaderConstants::TexCoord))
		;

	Core::RenderCommand render_command;
	render_command.set_vertices(vertex_allocation, vertices);

	return std::make_shared<Framework::Mesh>(render_command, vao_layout, vao_manager, get_name());
}
