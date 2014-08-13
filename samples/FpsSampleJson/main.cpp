#include "GM/Application/Main.h"

#include "GM/Core/Utilities/ShaderFactory.h"

#include "GM/Framework/Framework.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

#define COMPONENT_FPS_CONTROLLER "FPSController"
#define COMPONENT_IDLE_ROTATION "IdleRotation"

#define PROPERTY_CURSOR_DEADROOM "CursorDeadroom"
#define PROPERTY_PITCH_SENSITIVITY "PitchSensitivity"
#define PROPERTY_YAW_SENSITIVITY "YawSensitivity"
#define PROPERTY_SPEED "Speed"

using namespace GM;
using namespace Application;

class FPSControllerComponent : public Framework::Component < FPSControllerComponent >
{
public:
	FPSControllerComponent(const Framework::EntityPtr &owner, const MainPtr &app, const std::string &name = std::string())
		: Framework::Component< FPSControllerComponent >(owner, name), app(app.get())
	{
		position_property = owner->add(PROPERTY_POSITION, glm::vec3());
		orientation_property = owner->add(PROPERTY_ORIENTATION, glm::quat(
			glm::angleAxis(0.0f, glm::vec3(1, 0, 0)) *
			glm::angleAxis(0.0f, glm::vec3(0, 1, 0)) *
			glm::angleAxis(0.0f, glm::vec3(0, 0, 1))));

		world_matrix_property = owner->add(PROPERTY_WORLD_MATRIX, glm::mat4(1));

		cursor_deadroom_property = owner->add(PROPERTY_CURSOR_DEADROOM, 0.0125f);
		pitch_sensitivity_property = owner->add(PROPERTY_PITCH_SENSITIVITY, 1.0f);
		yaw_sensitivity_property = owner->add(PROPERTY_YAW_SENSITIVITY, 1.0f);
		max_vertical_angle_property = owner->add<float>(PROPERTY_MAX_VERTICAL_ANGLE, 85.0f);
		speed_property = owner->add<float>(PROPERTY_SPEED, 1000.0f);

		accum_vertical = 0.0f;
		accum_horizontal = 0.0f;
	}

	std::string get_type() const override { return get_static_type(); }

	void update(float elapsed_time) {
		auto &mouse_position = app->get_mouse_position();
		if (glm::abs(mouse_position.x) > cursor_deadroom_property || glm::abs(mouse_position.y) > cursor_deadroom_property) {
			auto vertical = mouse_position.x * pitch_sensitivity_property;
			auto horizontal = mouse_position.y * yaw_sensitivity_property;

			accum_vertical += vertical;
			accum_horizontal += horizontal;

			if (glm::degrees(accum_horizontal) > max_vertical_angle_property) {
				accum_horizontal = glm::radians(max_vertical_angle_property.get());
			}
			else if (glm::degrees(accum_horizontal) < -max_vertical_angle_property) {
				accum_horizontal = glm::radians(-max_vertical_angle_property.get());
			}
			if (glm::degrees(accum_vertical) > 360.0f) {
				accum_vertical -= glm::radians(360.0f);
			}
			else if (glm::degrees(accum_vertical) < 0.0f) {
				accum_vertical += glm::radians(360.0f);
			}

			auto angle_x = glm::angleAxis(accum_horizontal, glm::vec3(1, 0, 0));
			auto angle_y = glm::angleAxis(-accum_vertical, glm::vec3(0, 1, 0));

			orientation_property = glm::normalize(angle_y * angle_x);

			app->reset_mouse_position();
		}

		auto move_x = 0.0f;
		if (app->is_key_down(GLFW_KEY_A)) {
			move_x = -1;
		}
		else if (app->is_key_down(GLFW_KEY_D)) {
			move_x = 1;
		}

		auto move_y = 0.0f;
		if (app->is_key_down(GLFW_KEY_Z)) {
			move_y = -1;
		}
		else if (app->is_key_down(GLFW_KEY_X)) {
			move_y = 1;
		}

		auto move_z = 0.0f;
		if (app->is_key_down(GLFW_KEY_W)) {
			move_z = -1;
		}
		else if (app->is_key_down(GLFW_KEY_S)) {
			move_z = 1;
		}

		auto x_axis = glm::vec3(world_matrix_property.get()[0][0], world_matrix_property.get()[0][1], world_matrix_property.get()[0][2]);
		auto y_axis = glm::vec3(world_matrix_property.get()[1][0], world_matrix_property.get()[1][1], world_matrix_property.get()[1][2]);
		auto z_axis = glm::vec3(world_matrix_property.get()[2][0], world_matrix_property.get()[2][1], world_matrix_property.get()[2][2]);

		auto forward = -z_axis;
		auto fpforward = glm::cross(glm::vec3(0, 1, 0), x_axis);
		fpforward = glm::normalize(-fpforward);

		if (move_x != 0 || move_y != 0 || move_z != 0) {
			auto strafe_dir = glm::normalize(orientation_property.get() * glm::vec3(1, 0, 0));
			auto forward_dir = glm::normalize(orientation_property.get() * glm::vec3(0, 0, 1));
			auto move_dir = glm::normalize(x_axis * move_x + glm::vec3(0, 1, 0) * move_y + fpforward * move_z);
			position_property += move_dir * speed_property.get() * elapsed_time;
		}
	}

public:
	static std::string get_static_type() { return COMPONENT_FPS_CONTROLLER; }

private:
	Main *app;

	Framework::Property<glm::vec3> position_property;
	Framework::Property<glm::quat> orientation_property;
	Framework::Property<glm::mat4> world_matrix_property;
	Framework::Property<float> max_vertical_angle_property;
	Framework::Property<float> cursor_deadroom_property;
	Framework::Property<float> pitch_sensitivity_property;
	Framework::Property<float> yaw_sensitivity_property;
	Framework::Property<float> speed_property;

	float accum_vertical;
	float accum_horizontal;
};

class IdleRotationComponent : public Framework::Component < IdleRotationComponent >
{
public:
	IdleRotationComponent(const Framework::EntityPtr &owner, const std::string &name = std::string())
		: Framework::Component< IdleRotationComponent >(owner, name)
	{
		orientation_property = owner->add(PROPERTY_ORIENTATION, glm::quat(
			glm::angleAxis(0.0f, glm::vec3(1, 0, 0)) *
			glm::angleAxis(0.0f, glm::vec3(0, 1, 0)) *
			glm::angleAxis(0.0f, glm::vec3(0, 0, 1))));
	}

	std::string get_type() const override { return get_static_type(); }

	void update(float elapsed_time) {
		orientation_property = glm::rotate(orientation_property.get(), elapsed_time, glm::vec3(0, 1, 0));
	}

public:
	static std::string get_static_type() { return COMPONENT_IDLE_ROTATION; }

private:
	Framework::Property<glm::quat> orientation_property;
};

class MyComponentSerializer {
public:
	MyComponentSerializer(const MainPtr &app) : app(app) {
		slots.connect(
			app->get_entity_manager()->register_component_serializer_signal(),
			this, &MyComponentSerializer::create_and_add_component);
	}

	void create_and_add_component(const Framework::EntityPtr &owner, const std::string &type, const std::string &/*name*/) {
		if (type == Framework::Camera::get_static_type()) {
			owner->create_component<Framework::Camera>(app->get_render_system());
		}
		else if (type == Framework::Renderable::get_static_type()) {
			owner->create_component<Framework::Renderable>(app->get_render_system(), app->get_material_manager(), app->get_mesh_manager());
		}
		else if (type == Framework::Transform::get_static_type()) {
			owner->create_component<Framework::Transform>(app->get_scene_system());
		}
		else if (type == Framework::Light::get_static_type()) {
			owner->create_component<Framework::Light>(app->get_render_system());
		}
		else if (type == FPSControllerComponent::get_static_type()) {
			owner->create_component<FPSControllerComponent>(app);
		}
		else if (type == IdleRotationComponent::get_static_type()) {
			owner->create_component<IdleRotationComponent>();
		}
	}
private:
	MainPtr app;
	clan::SlotContainer slots;
};

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	auto entity_manager = app->get_entity_manager();
	auto component_serializer = std::make_shared<MyComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/litmesh");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/litmesh");
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
	auto light = entity_manager->create_entity("light");

	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("fps_camera", camera);
	entity_manager->apply("spaceship", spaceship);
	entity_manager->apply("light", light);

	// Set up the projection for the camera
	if (camera->has_component<Framework::Camera>()) {
		camera->get_component<Framework::Camera>()->set_projection(app->get_resolution());
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
