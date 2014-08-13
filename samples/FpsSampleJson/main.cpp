#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"
#include "GM/Samples/Starfield.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

// TODO: Must have a camera that can draw to an FBO (RTT) before we can truly do this...
//		 Then apply the result to a cube texture and slap that onto a skybox.
#define STARFIELD 0

using namespace GM;
using namespace Application;

void create_fullscreen_quad_mesh(const MainPtr &app)
{
	if (app->get_mesh_manager()->contains("fullscreen_quad"))
	{
		return;
	}

	struct MyVertex {
		glm::vec2 position;
	};

	const float s = 1.0f;
	std::vector<MyVertex> vertices{
			{{ -s, -s }},
			{{  s, -s }},
			{{ -s,  s }},
			{{  s,  s }}
	};

	auto vertex_allocation = app->get_buffer_manager()->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(vertex_allocation)
		.use_as(GL_ARRAY_BUFFER)
		.bind_interleaved(
		Core::VaoArg<glm::vec2>(Core::ShaderConstants::Position))
		;

	Core::RenderCommand render_command;
	render_command.set_vertices(vertex_allocation, vertices);

	auto mesh = std::make_shared<Framework::Mesh>(render_command, vao_layout, app->get_vao_manager(), "fullscreen_quad");
	app->get_mesh_manager()->add(mesh);
}

void create_skybox_mesh(float s, const MainPtr &app)
{
	if (app->get_mesh_manager()->contains("skybox"))
	{
		return;
	}

	struct MyVertex {
		glm::vec3 position;
	};

	Core::VaoLayout vao_layout;
	Core::RenderCommand render_command;

	std::vector<MyVertex> vertices {
			//X+
			{ { s,-s,-s } },
			{ { s,-s, s } },
			{ { s, s, s } },
			{ { s, s,-s } },
			
			//X-
			{ {-s, s,-s } },
			{ {-s, s, s } },
			{ {-s,-s, s } },
			{ {-s,-s,-s } },

			//Y+
			{ { s, s,-s } },
			{ { s, s, s } },
			{ {-s, s, s } },
			{ {-s, s,-s } },

			//Y-
			{ {-s,-s,-s } },
			{ {-s,-s, s } },
			{ { s,-s, s } },
			{ { s,-s,-s } },

			//Z+
			{ { s,-s, s } },
			{ {-s,-s, s } },
			{ {-s, s, s } },
			{ { s, s, s } },

			//Z-
			{ { s, s,-s } },
			{ {-s, s,-s } },
			{ {-s,-s,-s } },
			{ { s,-s,-s } }
	};

	std::vector<unsigned int> indices {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8,  9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	auto vertex_allocation = app->get_buffer_manager()->allocate_and_upload(vertices, GL_DYNAMIC_DRAW);
	vao_layout
		.for_buffer(vertex_allocation)
			.use_as(GL_ARRAY_BUFFER)
				.bind_interleaved(
					Core::VaoArg<glm::vec3>(Core::ShaderConstants::Position));
	render_command.set_vertices(vertex_allocation, vertices);

	auto index_allocation = app->get_buffer_manager()->allocate_and_upload(indices);
	vao_layout
		.for_buffer(index_allocation)
			.use_as(GL_ELEMENT_ARRAY_BUFFER);
	render_command.set_indices(index_allocation, indices);

	render_command.set_draw_mode(GL_TRIANGLES);

	auto mesh = std::make_shared<Framework::Mesh>(render_command, vao_layout, app->get_vao_manager(), "skybox");
	app->get_mesh_manager()->add(mesh);
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto entity_manager = app->get_entity_manager();
	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/fps");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/fps");
	auto mesh_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/mesh/samples/mesh");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures/samples/fps");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_mesh_manager()->add_templates(json_path + "/mesh_templates.json");
	app->get_mesh_manager()->set_mesh_path(mesh_path);
	app->get_texture_manager()->add_templates(json_path + "/texture_templates.json");
	app->get_texture_manager()->add_format_templates(json_path + "/texture_format_templates.json");
	app->get_texture_manager()->set_texture_path(texture_path);

	// Set up resources
	create_fullscreen_quad_mesh(app);
	create_skybox_mesh(49900, app);

	// Create our entities
	auto camera = entity_manager->create_entity("camera");
	auto spaceship = entity_manager->create_entity("spaceship");
	auto skybox = entity_manager->create_entity("skybox");
#if STARFIELD
	auto starfield = entity_manager->create_entity("starfield");
#endif

	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("fps_camera", camera);
	entity_manager->apply("spaceship", spaceship);
	entity_manager->apply("skybox", skybox);
#if STARFIELD
	entity_manager->apply("starfield", starfield);
#endif
	
	// Set up the projection for the camera
	if (camera->has_component<Framework::Camera>()) {
		auto camera_component = camera->get_component<Framework::Camera>();
		camera_component->set_projection(app->get_resolution());
#if STARFIELD
		if (starfield->has_component<Samples::StarfieldComponent>()) {
			starfield->get_component<Samples::StarfieldComponent>()->set_camera(camera_component);
		}
#endif
	}

	app->hide_cursor();

	// Set some run time limits
	float max_run_time = -1;
	float run_time = 0.f;

	auto update_slot = app->on_update().connect([&](float dt) mutable {
		run_time += dt;
		if ((max_run_time > 0 && run_time > max_run_time) || app->is_key_down(GLFW_KEY_ESCAPE))
		{
			app->stop_running();
			return;
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
