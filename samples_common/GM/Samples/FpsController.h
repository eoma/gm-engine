#pragma once

#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Samples/SampleDefinitionsComponentNames.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

namespace GM {
	namespace Samples {
		class FPSControllerComponent : public GM::Framework::Component < FPSControllerComponent >
		{
		public:
			FPSControllerComponent(const GM::Framework::EntityPtr &owner, const GM::Application::MainPtr &app, const std::string &name = std::string());

			std::string get_type() const override { return get_static_type(); }
			void update(float elapsed_time) override;

		public:
			static std::string get_static_type() { return COMPONENT_FPS_CONTROLLER; }

		private:
			Application::Main *app;

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
	}
}