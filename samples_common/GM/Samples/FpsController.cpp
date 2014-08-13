#include "FpsController.h"
#include "GM/Samples/SampleDefinitionsPropertyNames.h"
#include "GM/Application/Main.h"

using namespace GM;
using namespace Application;
using namespace Samples;

FPSControllerComponent::FPSControllerComponent(const Framework::EntityPtr &owner, const Application::MainPtr &app, const std::string &name)
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

void FPSControllerComponent::update(float elapsed_time) {
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
