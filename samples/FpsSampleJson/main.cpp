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

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto entity_manager = app->get_entity_manager();
	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/fps");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/fps");
	auto mesh_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/mesh/samples/mesh");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures/samples");

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
