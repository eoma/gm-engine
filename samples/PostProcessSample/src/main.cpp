#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"

#include "GM/Framework/Components/StandardPass.h"
#include "SobelFilter.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>


using namespace GM;
using namespace Application;

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto entity_manager = app->get_entity_manager();
	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/mesh");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/mesh");
	auto mesh_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/mesh/samples/mesh");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_mesh_manager()->add_templates(json_path + "/mesh_templates.json");
	app->get_mesh_manager()->set_mesh_path(mesh_path);

	// Create our entities
	auto camera = entity_manager->create_entity("camera");
	auto spaceship = entity_manager->create_entity("spaceship");


	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("camera", camera);
	entity_manager->apply("spaceship", spaceship);

	// Set up standard pass and post process
	camera->create_component<Framework::StandardPass>();
	camera->create_component<SobelFilter>();

	// Set up the projection for the camera
	if (camera->has_component<Framework::Camera>()) {
		camera->get_component<Framework::Camera>()->set_projection(app->get_resolution());
	}

	// Set some run time limits
	float max_run_time = -1.f;
	unsigned int max_draws = 0;
	float run_time = 0.f;
	unsigned int draws = 0;

	auto update_slot = app->on_update().connect([&](float dt) mutable {
		run_time += dt;
		draws += 1;
		if ((draws > max_draws && max_draws > 0) || (run_time > max_run_time && max_run_time > 0) || app->is_key_down(GLFW_KEY_ESCAPE))
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
