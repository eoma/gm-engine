#include "GM/Framework/Primitives/TrianglePrimitive.h"
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

TrianglePrimitive::TrianglePrimitive()
	: IPrimitive(GM_PRIMITIVE_TRIANGLE)
{
}

MeshPtr TrianglePrimitive::create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) {
	std::vector<glm::vec3> vertices{
			{ -1.0f, -1.0f, 0.0f },
			{ 1.0f, -1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
	};

	auto buffer_allocation = buffer_manager->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(buffer_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind<glm::vec3>(Core::ShaderConstants::Position)
		;

	Core::RenderCommand render_command;
	render_command.set_vertices(buffer_allocation, vertices);

	return std::make_shared<Framework::Mesh>(render_command, vao_layout, vao_manager, get_name());
}
