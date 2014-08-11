#include "GM/Application/Main.h"

#include "GM/Core/Utilities/ShaderFactory.h"

#include "GM/Framework/Framework.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

#define COMPONENT_IDLE_ROTATION "IdleRotation"

using namespace GM;
using namespace Application;

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
	entity_manager->apply("camera", camera);
	entity_manager->apply("spaceship", spaceship);
	entity_manager->apply("light", light);

	// Set up the projection for the camera
	if (camera->has_component<Framework::Camera>()) {
		camera->get_component<Framework::Camera>()->set_projection(app->get_resolution());
	}

	auto camera_pos = camera->add<glm::vec3>(PROPERTY_POSITION, glm::vec3());

	// Set some run time limits
	float max_run_time = 60.f;
	float run_time = 0.f;

	float speed = 1000;

	auto update_slot = app->on_update().connect([&](float dt) mutable {
		run_time += dt;
		if (run_time > max_run_time || app->is_key_down(GLFW_KEY_ESCAPE))
		{
			app->stop_running();
			return;
		}

		if (app->is_key_down(GLFW_KEY_W))
		{
			camera_pos += glm::vec3(0,0,-1) * speed * dt;
		}
		else if (app->is_key_down(GLFW_KEY_A))
		{
			camera_pos += glm::vec3(-1,0,0) * speed * dt;
		}
		else if (app->is_key_down(GLFW_KEY_S))
		{
			camera_pos += glm::vec3(0,0,1) * speed * dt;
		}
		else if (app->is_key_down(GLFW_KEY_D))
		{
			camera_pos += glm::vec3(1,0,0) * speed * dt;
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
