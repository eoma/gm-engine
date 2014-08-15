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

int currently_selected_part;

enum RobotParts {
	ROBOT = 0,
	BASE_LEFT,
	BASE_RIGHT,
	ARM_LOWER,
	ARM_UPPER,
	HEAD,
	CLAW_LOWER_LEFT,
	CLAW_LOWER_RIGHT,
	CLAW_UPPER_LEFT,
	CLAW_UPPER_RIGHT,
	ROBOT_PARTS_COUNT
};

std::string to_robot_part_name(RobotParts robot_part) {
	switch (robot_part) {
	case ROBOT:
		return "robot";
	case BASE_LEFT:
		return "robot_base_left";
	case BASE_RIGHT:
		return "robot_base_right";
	case ARM_LOWER:
		return "robot_arm_lower";
	case ARM_UPPER:
		return "robot_arm_upper";
	case HEAD:
		return "robot_head";
	case CLAW_LOWER_LEFT:
		return "robot_claw_lower_left";
	case CLAW_LOWER_RIGHT:
		return "robot_claw_lower_right";
	case CLAW_UPPER_LEFT:
		return "robot_claw_upper_left";
	case CLAW_UPPER_RIGHT:
		return "robot_claw_upper_right";
	default:
		return "unknown";
	};
}

Framework::Transform *find_part(Framework::Transform *robot_or_part, RobotParts robot_part) {
	std::string part_name = to_robot_part_name(robot_part);
	if (part_name == robot_or_part->get_owner()->get_name())
		return robot_or_part;

	for (auto iter = robot_or_part->get_children().begin(); iter != robot_or_part->get_children().end(); ++iter) {
		auto part = (*iter);
		if (part->get_owner()->get_name() == part_name) {
			return part;
		}
		
		auto was_part_found = find_part(part, robot_part);
		if (was_part_found != nullptr)
			return was_part_found;
	}

	return nullptr;
}

Framework::TransformPtr create_robot(Framework::EntityManagerPtr entity_manager) {
	// Create empty entities
	auto robot_entity = entity_manager->create_entity(					to_robot_part_name(RobotParts::ROBOT) );
	auto robot_base_left_entity = entity_manager->create_entity(		to_robot_part_name(RobotParts::BASE_LEFT));
	auto robot_base_right_entity = entity_manager->create_entity(		to_robot_part_name(RobotParts::BASE_RIGHT));
	auto robot_arm_lower_entity = entity_manager->create_entity(		to_robot_part_name(RobotParts::ARM_LOWER));
	auto robot_arm_upper_entity = entity_manager->create_entity(		to_robot_part_name(RobotParts::ARM_UPPER));
	auto robot_head_entity = entity_manager->create_entity(				to_robot_part_name(RobotParts::HEAD));
	auto robot_claw_upper_left_entity = entity_manager->create_entity(	to_robot_part_name(RobotParts::CLAW_UPPER_LEFT));
	auto robot_claw_upper_right_entity = entity_manager->create_entity(	to_robot_part_name(RobotParts::CLAW_UPPER_RIGHT));
	auto robot_claw_lower_left_entity = entity_manager->create_entity(	to_robot_part_name(RobotParts::CLAW_LOWER_LEFT));
	auto robot_claw_lower_right_entity = entity_manager->create_entity(	to_robot_part_name(RobotParts::CLAW_LOWER_RIGHT));

	// Apply templates
	entity_manager->apply("robot", robot_entity);
	entity_manager->apply("robot_base", robot_base_left_entity);
	entity_manager->apply("robot_base", robot_base_right_entity);
	entity_manager->apply("robot_arm", robot_arm_lower_entity);
	entity_manager->apply("robot_arm", robot_arm_upper_entity);

	// Extract scenegraph components (Transform)
	auto robot = robot_entity->get_component<Framework::Transform>();
	auto robot_base_left = robot_base_left_entity->get_component<Framework::Transform>();
	auto robot_base_right = robot_base_right_entity->get_component<Framework::Transform>();
	auto robot_arm_lower = robot_arm_lower_entity->get_component<Framework::Transform>();
	auto robot_arm_upper = robot_arm_upper_entity->get_component<Framework::Transform>();

	// Assemble the robot
	robot->add_child(robot_base_left);
	robot->add_child(robot_base_right);
	robot->add_child(robot_arm_lower);
	robot_arm_lower->add_child(robot_arm_upper);

	// Position assembled robot parts
	robot_base_left->translate(glm::vec3(-1, 0, 0));
	robot_base_right->translate(glm::vec3(1, 0, 0));
	robot_arm_upper->translate(glm::vec3(0, 7.5f, 0));

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

	currently_selected_part = 0;
	auto robot = create_robot(entity_manager);
	auto robot_arm_lower = find_part(robot.get(), RobotParts::ARM_LOWER);
	auto robot_arm_upper = find_part(robot.get(), RobotParts::ARM_UPPER);
	
	app->hide_cursor();
	auto keyboard_slot = app->sign_keyboard().connect(
		[&](int key, int scancode, int action, int mods) {
			bool new_robot_part_selected = false;

			// Select a robot part
			if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
				currently_selected_part++;
				new_robot_part_selected = true;
			}
			else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
				currently_selected_part--;
				new_robot_part_selected = true;
			}

			if (new_robot_part_selected) {
				
				// Cycle the selected parts within bounds
				if (currently_selected_part >= RobotParts::ROBOT_PARTS_COUNT) {
					currently_selected_part = 0;
				}
				else if (currently_selected_part < 0) {
					currently_selected_part = RobotParts::ROBOT_PARTS_COUNT - 1;
				}
				std::cout << "Selected Robot Part: " << to_robot_part_name((RobotParts)currently_selected_part) << std::endl;
			}
		});

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

		// If the robot's root is selected, we move the robot about
		if (currently_selected_part == RobotParts::ROBOT) {
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
		}

		// If the base of the robot is selected, we rotate the robot left or right
		else if (currently_selected_part == RobotParts::BASE_LEFT || currently_selected_part == RobotParts::BASE_RIGHT) {
			if (app->is_key_down(GLFW_KEY_LEFT)) {
				robot->rotate(glm::angleAxis(dt, glm::vec3(0, 1, 0)));
			}
			else if (app->is_key_down(GLFW_KEY_RIGHT)) {
				robot->rotate(glm::angleAxis(dt, glm::vec3(0, -1, 0)));
			}
		}

		// If the lower arm of the robot is selected, we rotate the lower arm up or down
		else if (robot_arm_lower && currently_selected_part == RobotParts::ARM_LOWER)
		{
			if (app->is_key_down(GLFW_KEY_UP)) {
				robot_arm_lower->rotate(glm::angleAxis(dt, glm::vec3(1, 0, 0)));
			}
			else if (app->is_key_down(GLFW_KEY_DOWN)) {
				robot_arm_lower->rotate(glm::angleAxis(dt, glm::vec3(-1, 0, 0)));
			}
		}

		// If the upper arm of the robot is selected, we rotate the lower arm up or down
		else if (robot_arm_upper && currently_selected_part == RobotParts::ARM_UPPER)
		{
			if (app->is_key_down(GLFW_KEY_UP)) {
				robot_arm_upper->rotate(glm::angleAxis(dt, glm::vec3(1, 0, 0)));
			}
			else if (app->is_key_down(GLFW_KEY_DOWN)) {
				robot_arm_upper->rotate(glm::angleAxis(dt, glm::vec3(-1, 0, 0)));
			}
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
