#include "GM/Application/Main.h"

#include "GM/Core/Utilities/ShaderFactory.h"

#include "GM/Framework/Entity.h"

#include "GM/Framework/EntityManager.h"
#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/MaterialManager.h"
#include "GM/Framework/Managers/MeshManager.h"
#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Utilities/Mesh.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

#define POSITION 0

using namespace GM;
using namespace Application;

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto render_system = app->get_render_system();

	auto entity_manager = app->get_entity_manager();
	auto vao_manager = app->get_vao_manager();
	auto buffer_manager = app->get_buffer_manager();
	auto mesh_manager = app->get_mesh_manager();
	auto material_manager = app->get_material_manager();

	std::vector<glm::vec3> vertices {
		{-1.0f, -1.0f, 0.0f},
		{1.0f, -1.0f, 0.0f},
		{0.0f,  1.0f, 0.0f},
	};

	auto buffer_allocation = buffer_manager->allocate_and_upload(vertices);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(buffer_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind<glm::vec3>(POSITION)
	;

	Core::RenderCommand render_command(false, vertices.size(), 0, buffer_allocation.offset / sizeof(glm::vec3));

	auto mesh = std::make_shared<Framework::Mesh>("triangle", render_command, vao_layout, vao_manager);
	mesh_manager->add(mesh->get_name(), mesh);

	typedef std::string s;

	Core::ShaderPtr shader = Core::ShaderFactory::make_program({
		Core::ShaderSource("vertex",
			s("#version 330\n") +
			s("#define POSITION 0\n") +
			s("layout(location = POSITION) in vec3 position;\n") +
			s("void main() {\n") + 
			s("    gl_Position = vec4(position, 1.0);\n") +
			s("}\n")
			,
			GL_VERTEX_SHADER),

		Core::ShaderSource("fragment",
			s("#version 330\n") + 
			s("out vec3 color;\n") +
			s("void main() {\n") +
			s("    color = vec3(1,0,0);\n") + 
			s("}\n"),
			GL_FRAGMENT_SHADER)
	});

	auto material = std::make_shared<Framework::Material>(shader, "triangle");
	material_manager->add(material->get_name(), material);

	auto entity = entity_manager->create_entity("entity");
	entity->create_component<Framework::Renderable>(render_system, material_manager, mesh_manager);
	entity->create_component<Framework::Camera>(render_system);
	entity->get<std::string>("MeshName") = "triangle";
	entity->get<std::string>("MaterialName") = "triangle";


	unsigned int num_renders = 1*30;
	unsigned int count = 0;

	auto update_slot = app->on_update().connect([&](float /*value*/) mutable {
		++count;
		if (count >= num_renders)
		{
			app->stop_running();
		}
	});

	app->run();

	return true;
}

int main() {
	
	bool successful = mainTest();

#ifdef WIN32
	system("pause");
#endif

	if (successful) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}
