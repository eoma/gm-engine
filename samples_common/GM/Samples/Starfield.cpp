#include "Starfield.h"
#include "GM/Samples/SampleDefinitionsPropertyNames.h"

using namespace GM;
using namespace Samples;

StarfieldComponent::StarfieldComponent(const GM::Framework::EntityPtr &owner, GM::Application::Main *app, const std::string &name)
	: Framework::Component< StarfieldComponent >(owner, name), app(app), last_elapsed_time(0.0f)
{

}

void StarfieldComponent::update(float elapsed_time) {
	last_elapsed_time = elapsed_time;

	// TODO: Better error reporting for missing stuff...
	if (renderable == nullptr) {
		if (owner->has_component<Framework::Renderable>()) {
			renderable = owner->get_component<Framework::Renderable>();
		}
		else {
			return;
		}

		// TODO: When we start rendering this starfield to an FBO, we should probably reduce the size of the FBO viewport to
		//		 something like 1/4th the actual resolution to save framerate.
		auto viewport_resolution = glm::vec2(app->get_resolution().x * 0.995f, app->get_resolution().y * 0.49f);
		auto resolution = glm::vec2(viewport_resolution.x / (float)app->get_resolution().x, viewport_resolution.y / (float)app->get_resolution().y);

		if (renderable->get_material()->has_property("resolution")) {
			resolution_property = renderable->get_material()->get<glm::vec2>("resolution");
			resolution_property = resolution;
		}
		if (renderable->get_material()->has_property("inv_resolution")) {
			inv_resolution_property = renderable->get_material()->get<glm::vec2>("inv_resolution");
			inv_resolution_property = glm::vec2(1.0f - resolution.x, 1.0f - resolution.y);
		}
		if (renderable->get_material()->has_property("origin")) {
			origin_property = renderable->get_material()->get<glm::vec3>("origin");
			if (camera != nullptr) {
				auto camera_owner = const_cast<Framework::Entity*>(camera->get_owner());
				slots.connect(camera_owner->get<glm::vec3>(PROPERTY_POSITION).value_changed(),
					[&](const glm::vec3 &, const glm::vec3 &new_value) {
						origin_property = new_value * 0.1f * last_elapsed_time;
				});
			}
		}
		if (renderable->get_material()->has_property("rotate")) {
			rotate_property = renderable->get_material()->get<glm::mat2>("rotate");
			if (camera != nullptr) {
				auto camera_owner = const_cast<Framework::Entity*>(camera->get_owner());
				slots.connect(camera_owner->get<glm::quat>(PROPERTY_ORIENTATION).value_changed(),
					[&](const glm::quat &, const glm::quat &new_value) {
						auto rot3 = glm::toMat3(new_value);
						rotate_property = glm::mat2(rot3);
				});
			}
		}
		if (renderable->get_material()->has_property("zoom")) {
			zoom_property = renderable->get_material()->get<float>("zoom");
			zoom_property = 0.5f;
		}
	}

	if (app->is_key_down(GLFW_KEY_C)) {
		zoom_property -= 0.001f;
		std::cout << "ZOOM: " << zoom_property.get() << std::endl;
	}
	else if (app->is_key_down(GLFW_KEY_V)) {
		zoom_property += 0.001f;
		std::cout << "ZOOM: " << zoom_property.get() << std::endl;
	}
}
