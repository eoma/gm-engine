#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"

#include "SimpleShadowMap.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>


using namespace GM;
using namespace Application;

void set_up_component_creator_functions(MainPtr &app)
{
	app->get_component_serializer()->add_component_creator(SimpleShadowMap::get_static_type(), 
		[app](const Framework::EntityPtr &owner) {
			owner->create_component<SimpleShadowMap>(app->get_texture_manager());
		});

	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);
}

void set_up_resource_paths(const MainPtr &app)
{
	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/simpleshadowmap");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/simpleshadowmap");
	auto mesh_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/mesh/samples/mesh");

	// Set up resource data
	app->get_entity_manager()->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_mesh_manager()->add_templates(json_path + "/mesh_templates.json");
	app->get_mesh_manager()->set_mesh_path(mesh_path);

}

clan::Slot set_up_common_update_function(const MainPtr &app, bool stop_on_escape = true, float max_run_time = -1.0f, unsigned int max_draws = 0)
{
	float run_time = 0.f;
	unsigned int draws = 0;

	return app->on_update().connect([=](float dt) mutable {
		run_time += dt;
		draws += 1;
		if ((stop_on_escape && app->is_key_down(GLFW_KEY_ESCAPE))
		    || (draws > max_draws && max_draws > 0)
		    || (run_time > max_run_time && max_run_time > 0))
		{
			app->stop_running();
		}
	});
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	set_up_component_creator_functions(app);
	set_up_resource_paths(app);

	auto entity_manager = app->get_entity_manager();

	// Create our entities
	//auto shadow_camera = entity_manager->create_entity("shadow_camera");
	auto camera = entity_manager->create_entity("camera");
	auto light_and_shadow = entity_manager->create_entity("light_and_shadow");
	auto surface = entity_manager->create_entity("surface");
	auto spaceship = entity_manager->create_entity("spaceship");
	auto rotator = entity_manager->create_entity("rotator");
	auto small_cube = entity_manager->create_entity("small_cube");

	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("light_and_shadow", light_and_shadow);
	entity_manager->apply("surface", surface);
	entity_manager->apply("camera", camera);
	entity_manager->apply("spaceship", spaceship);
	entity_manager->apply("rotator", rotator);
	entity_manager->apply("small_cube", small_cube);

	rotator->get_component<Framework::Transform>()->add_child(light_and_shadow->get_component<Framework::Transform>());
	light_and_shadow->get_component<Framework::Transform>()->add_child(small_cube->get_component<Framework::Transform>());

	clan::SlotContainer slots;

	auto ShadowView = light_and_shadow->get<glm::mat4>(GM_PROPERTY_VIEW_MATRIX);
	auto ShadowProjection = light_and_shadow->get<glm::mat4>(GM_PROPERTY_PROJECTION_MATRIX);

	// Update all of the materials containing a reference to a shadow uniform.
	// Improvised version of a "system".
	// TODO: Better version of this.
	slots.connect(app->on_prepare(), [&]() {
		glm::mat4 bias(0.5);
		bias[3] = glm::vec4(0.5, 0.5, 0.5, 1.0);
		//bias = glm::transpose(bias);
		glm::mat4 view_projection = bias * ShadowProjection.get() * ShadowView.get();

		for (const auto &material_iter : app->get_material_manager()->get_materials())
		{
			const auto &material = material_iter.second;
			if (material->has_property("ShadowVP"))
			{
				material->get<glm::mat4>("ShadowVP") = view_projection;
			}
		}
	});

	app->get_render_system()->resize(app->get_resolution().x, app->get_resolution().y);

	auto update_slot = set_up_common_update_function(app, true, -1.f, 0);

	// Force update of all world matrices in the scene system.
	app->get_scene_system()->prepare(true);

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
