#include "GM/Application/Application.h"
#include "GM/Framework/Framework.h"

// Very basic example.

int main() {
	using namespace GM;
	Application::MainPtr app = std::make_shared<Application::Main>("My project");

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl");
	auto mesh_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/mesh");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures");

	app->get_entity_manager()->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_mesh_manager()->add_templates(json_path + "/mesh_templates.json");
	app->get_mesh_manager()->set_mesh_path(mesh_path);
	app->get_texture_manager()->add_templates(json_path + "/texture_templates.json");
	app->get_texture_manager()->add_format_templates(json_path + "/texture_format_templates.json");
	app->get_texture_manager()->set_texture_path(texture_path);

	// Set up our entities
	Framework::EntityManagerPtr entity_manager = app->get_entity_manager();

	Framework::EntityPtr camera = entity_manager->create_entity("camera");
	entity_manager->apply("camera", camera); // apply camera entity template

	Framework::EntityPtr cube = entity_manager->create_entity("cube");
	entity_manager->apply("metal_cube", cube); // apple metal_cube entity template

	Framework::EntityPtr sphere = entity_manager->create_entity("mars_sphere");
	entity_manager->apply("mars_sphere", sphere);// ....

	// Set up the projection for the camera
	if (camera->has_component<Framework::Camera>()) {
		auto camera_component = camera->get_component<Framework::Camera>();
		camera_component->set_projection(app->get_resolution());
	}

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

	return 0;
}
