#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>


using namespace GM;
using namespace Application;

Framework::TransformPtr create_robot(Framework::EntityManagerPtr entity_manager) {
	// Create empty entities
	auto robot_entity = entity_manager->create_entity("robot");
	auto robot_base_left_entity = entity_manager->create_entity("robot_base_left");
	auto robot_base_right_entity = entity_manager->create_entity("robot_base_right");
	auto robot_arm_lower_entity = entity_manager->create_entity("robot_arm_lower");

	// Apply templates
	entity_manager->apply("robot", robot_entity);
	entity_manager->apply("robot_base", robot_base_left_entity);
	entity_manager->apply("robot_base", robot_base_right_entity);
	entity_manager->apply("robot_arm", robot_arm_lower_entity);

	// Extract scenegraph components (Transform)
	auto robot = robot_entity->get_component<Framework::Transform>();
	auto robot_base_left = robot_base_left_entity->get_component<Framework::Transform>();
	auto robot_base_right = robot_base_right_entity->get_component<Framework::Transform>();
	auto robot_arm_lower = robot_arm_lower_entity->get_component<Framework::Transform>();

	// Assemble the robot
	robot->add_child(robot_base_left);
	robot->add_child(robot_base_right);
	robot->add_child(robot_arm_lower);

	// Position assembled robot parts
	robot_base_left->translate(glm::vec3(-1, 0, 0));
	robot_base_right->translate(glm::vec3(1, 0, 0));

	return robot;
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto entity_manager = app->get_entity_manager();
	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/robot");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/robot");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures/samples");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_texture_manager()->add_templates(json_path + "/texture_templates.json");
	app->get_texture_manager()->add_format_templates(json_path + "/texture_format_templates.json");
	app->get_texture_manager()->set_texture_path(texture_path);

	// Create our entities
	auto camera = entity_manager->create_entity("camera");
	auto skybox = entity_manager->create_entity("skybox");
	auto floor = entity_manager->create_entity("floor");

	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("fps_camera", camera);
	entity_manager->apply("skybox", skybox);
	entity_manager->apply("floor", floor);
	
	// Set up the projection for the camera
	if (camera->has_component<Framework::Camera>()) {
		auto camera_component = camera->get_component<Framework::Camera>();
		camera_component->set_projection(app->get_resolution());
	}

	auto robot = create_robot(entity_manager);
	
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

		if (app->is_key_down(GLFW_KEY_UP)) {
			robot->translate(glm::vec3(0, 0, -1) * dt);
		}
		else if (app->is_key_down(GLFW_KEY_DOWN)) {
			robot->translate(glm::vec3(0, 0, 1) * dt);
		}
		if (app->is_key_down(GLFW_KEY_LEFT)) {
			robot->translate(glm::vec3(-1, 0, 0) * dt);
		}
		else if (app->is_key_down(GLFW_KEY_RIGHT)) {
			robot->translate(glm::vec3(1, 0, 0) * dt);
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
