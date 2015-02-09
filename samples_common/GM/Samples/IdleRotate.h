#pragma once

#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Samples/SampleDefinitionsComponentNames.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

namespace GM {
	namespace Samples {
		class IdleRotationComponent : public GM::Framework::Component < IdleRotationComponent >
		{
		public:
			IdleRotationComponent(const GM::Framework::EntityPtr &owner, const std::string &name = std::string());
			
			std::string get_type() const override { return get_static_type(); }
			void update(float elapsed_time) override;

		public:
			static std::string get_static_type() { return GM_COMPONENT_IDLE_ROTATION; }

		private:
			Framework::Property<glm::quat> orientation_property;
			Framework::Property<glm::vec3> rotation_axis_property;
			Framework::Property<float> rotation_frequency_property;
		};
	}
}
