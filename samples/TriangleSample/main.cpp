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

void create_triangle_mesh(const MainPtr &app)
{
	if (app->get_mesh_manager()->contains("triangle"))
	{
		return;
	}

	std::vector<glm::vec3> vertices {
		{-1.0f, -1.0f, 0.0f},
		{1.0f, -1.0f, 0.0f},
		{0.0f,  1.0f, 0.0f},
	};

	auto buffer_allocation = app->get_buffer_manager()->allocate_and_upload(vertices);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(buffer_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind<glm::vec3>(POSITION)
	;

	Core::RenderCommand render_command(false, vertices.size(), 0, buffer_allocation.offset / sizeof(glm::vec3));

	auto mesh = std::make_shared<Framework::Mesh>("triangle", render_command, vao_layout, app->get_vao_manager());
	app->get_mesh_manager()->add(mesh->get_name(), mesh);
}

void create_shader(const MainPtr &app)
{
	if (app->get_shader_manager()->contains("diffuse"))
	{
		return;
	}

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
			s("uniform vec3 diffuse = vec3(0, 0, 0);") +
			s("void main() {\n") +
			s("    color = diffuse;\n") +
			s("}\n")
			,
			GL_FRAGMENT_SHADER)
	});

	app->get_shader_manager()->add("diffuse", shader);
}

void create_red_material(const MainPtr &app)
{
	if (app->get_material_manager()->contains("red_diffuse"))
	{
		return;
	}
	auto material = app->get_material_manager()->get_or_create("red_diffuse", "diffuse");
	material->add_uniform<glm::vec3>("diffuse", glm::vec3(1.f, 0.f, 0.f));
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto render_system = app->get_render_system();

	auto entity_manager = app->get_entity_manager();
	auto mesh_manager = app->get_mesh_manager();
	auto material_manager = app->get_material_manager();

	// Set up resources
	create_triangle_mesh(app);
	create_shader(app);
	create_red_material(app);

	// Create our entity and add components so that it is possible to render
	auto entity = entity_manager->create_entity("entity");
	entity->create_component<Framework::Renderable>(render_system, material_manager, mesh_manager);
	entity->create_component<Framework::Camera>(render_system);

	// Set up what resources our renderable is supposed to use
	entity->get<std::string>("MeshName") = "triangle";
	entity->get<std::string>("MaterialName") = "red_diffuse";

	// Set some run time limits
	float max_run_time = 1.0f;
	float run_time = 0.f;

	auto update_slot = app->on_update().connect([&](float dt) mutable {
		run_time += dt;
		if (run_time > max_run_time)
		{
			app->stop_running();
		}
	});

	// Start rendering
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
