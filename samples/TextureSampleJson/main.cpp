#include "GM/Application/Main.h"

#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/ShaderFactory.h"

#include "GM/Framework/Framework.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

using namespace GM;
using namespace Application;

void create_quad_mesh(const MainPtr &app)
{
	if (app->get_mesh_manager()->contains("quad"))
	{
		return;
	}

	struct MyVertex {
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	std::vector<MyVertex> vertices{
			{ { -0.5f, -0.5f, 0.0f },	{ 0, 0 } },
			{ { 0.5f, -0.5f, 0.0f },	{ 1, 0 } },
			{ { -0.5f, 0.5f, 0.0f },	{ 0, 1 } },
			{ { 0.5f, 0.5f, 0.0f },		{ 1, 1 } }
	};

	auto vertex_allocation = app->get_buffer_manager()->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);

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

	auto mesh = std::make_shared<Framework::Mesh>(render_command, vao_layout, app->get_vao_manager(), "quad");
	app->get_mesh_manager()->add(mesh);
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto entity_manager = app->get_entity_manager();

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/texture");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/texture");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures/samples/texture");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_texture_manager()->add_templates(json_path + "/texture_templates.json");
	app->get_texture_manager()->add_format_templates(json_path + "/texture_format_templates.json");
	app->get_texture_manager()->set_texture_path(texture_path);

	// Set up resources
	create_quad_mesh(app);

	// Create our entity
	auto entity = entity_manager->create_entity("entity");

	// Apply the entity template "quad", defined in entity_templates.json
	entity_manager->apply("quad", entity);

	// Set some run time limits
	float max_run_time = 2.f;
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
