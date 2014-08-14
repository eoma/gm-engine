#pragma once

#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Samples/SampleDefinitionsComponentNames.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

namespace GM {
	namespace Samples {
		class StarfieldComponent : public GM::Framework::Component < StarfieldComponent >
		{
		public:
			StarfieldComponent(const GM::Framework::EntityPtr &owner, GM::Application::Main *app, const std::string &name = std::string());
			
			std::string get_type() const override { return get_static_type(); }
			void update(float elapsed_time) override;

			void set_camera(const Framework::CameraPtr &camera) { this->camera = camera; }

		public:
			static std::string get_static_type() { return GM_COMPONENT_STARFIELD; }

		private:
			GM::Application::Main *app;
			Framework::RenderablePtr renderable;
			Framework::CameraPtr camera;

			float last_elapsed_time;

			Framework::Property<glm::vec2> resolution_property;
			Framework::Property<glm::vec2> inv_resolution_property;

			Framework::Property<glm::vec3> origin_property;
			Framework::Property<glm::mat2> rotate_property;

			Framework::Property<float> zoom_property;

			clan::SlotContainer slots;
		};
	}
}