#include "GM/Framework/Primitives/SkyboxPrimitive.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/VaoManager.h"
#include "GM/Framework/Utilities/Mesh.h"
#include "GM/Framework/DefinitionsPrimitiveNames.h"

#include "GM/Core/Utilities/VaoLayout.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/RenderCommand.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/gl3w.h>

#include <vector>
using namespace GM;
using namespace Framework;

SkyboxPrimitive::SkyboxPrimitive()
	: IPrimitive(GM_PRIMITIVE_SKYBOX)
{
}

MeshPtr SkyboxPrimitive::create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) {
	struct MyVertex {
		glm::vec3 position;
	};

	Core::VaoLayout vao_layout;
	Core::RenderCommand render_command;

	float s = 0.5f;
	std::vector<MyVertex> vertices{
		//X+
			{ { s, -s, -s } },
			{ { s, -s, s } },
			{ { s, s, s } },
			{ { s, s, -s } },

			//X-
			{ { -s, s, -s } },
			{ { -s, s, s } },
			{ { -s, -s, s } },
			{ { -s, -s, -s } },

			//Y+
			{ { s, s, -s } },
			{ { s, s, s } },
			{ { -s, s, s } },
			{ { -s, s, -s } },

			//Y-
			{ { -s, -s, -s } },
			{ { -s, -s, s } },
			{ { s, -s, s } },
			{ { s, -s, -s } },

			//Z+
			{ { s, -s, s } },
			{ { -s, -s, s } },
			{ { -s, s, s } },
			{ { s, s, s } },

			//Z-
			{ { s, s, -s } },
			{ { -s, s, -s } },
			{ { -s, -s, -s } },
			{ { s, -s, -s } }
	};

	std::vector<unsigned int> indices{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	auto vertex_allocation = buffer_manager->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);
	vao_layout
		.for_buffer(vertex_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind_interleaved(
					Core::VaoArg<glm::vec3>(Core::ShaderConstants::Position));
	render_command.set_vertices(vertex_allocation, vertices);

	auto index_allocation = buffer_manager->allocate_and_upload(indices);
	vao_layout
		.for_buffer(index_allocation)
			.use_as(GL_ELEMENT_ARRAY_BUFFER);
	render_command.set_indices(index_allocation, indices);

	render_command.set_draw_mode(GL_TRIANGLES);

	return std::make_shared<Framework::Mesh>(render_command, vao_layout, vao_manager, get_name());
}