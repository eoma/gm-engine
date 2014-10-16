#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Core/Utilities/ShaderConstants.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>
#include <cstdlib>

#include "TerrainMaker.h"

using namespace GM;
using namespace Application;

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 4, 1);

	auto entity_manager = app->get_entity_manager();
	auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/terrain");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/terrain");
	auto texture_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/textures/samples");
	auto mesh_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/mesh/samples");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_mesh_manager()->add_templates(json_path + "/mesh_templates.json");
	app->get_mesh_manager()->set_mesh_path(mesh_path);
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
	app->get_texture_manager()->add_templates(json_path + "/texture_templates.json");
	app->get_texture_manager()->add_format_templates(json_path + "/texture_format_templates.json");
	app->get_texture_manager()->set_texture_path(texture_path);

	// Setup resources
	create_triangle_mesh(app, 1024, 1024, false); // True when tesselation is active, false when not... (Need to make sure the tesselation material is active before this is set to true though).

	// Create our entities
	auto camera_entity = entity_manager->create_entity("camera");
	auto skybox = entity_manager->create_entity("skybox");
	auto terrain = entity_manager->create_entity("terrain");
	//auto light = entity_manager->create_entity("light");
	auto rock = entity_manager->create_entity("rock");

	// Apply an entity template, as defined in entity_templates.json
	entity_manager->apply("fps_camera", camera_entity);
	entity_manager->apply("skybox", skybox);
	entity_manager->apply("terrain", terrain);
	//entity_manager->apply("light", light);
	entity_manager->apply("rock", rock);

	// Instancing example
	{
		auto instanced_rock = entity_manager->create_entity("instanced_rock");
		entity_manager->apply("rock_instanced", instanced_rock);

		// Our instance data structure
		struct MyInstanceData {
			glm::vec3 position;
		};

		std::vector<MyInstanceData> instance_collection;

		for (unsigned int x = 0; x < 10; ++x) {
			for (unsigned int y = 0; y < 10; ++y) {
				// Generate instance data
				instance_collection.push_back(MyInstanceData{
					{ x, 0 , y }
				});
			}
		}
		
		if (instanced_rock->has_component<Framework::Renderable>()) {
			// We can fetch renderable, and thus mesh
			auto renderable = instanced_rock->get_component<Framework::Renderable>();

			auto original_mesh = renderable->get_mesh(); // We will not modify the original mesh

			if (original_mesh != nullptr) {
				// Can not set up instancing if mesh has not been set

				// Upload our data, alternatively we could manually allocate using
				// auto allocation = app->get_buffer_manager()->allocate<MyInstanceData>(max_number_of_instances);
				// allocation.upload(instance_collection);
				Core::BufferAllocation instance_allocation = app->get_buffer_manager()->allocate_and_upload(instance_collection);

				// We need to get a copy of the vao layout without any previous instances, we have no
				// guarantees the format is the same
				Core::VaoLayout instanced_layout = original_mesh->get_vao_layout().get_copy_without_instancing();

				// The instance vao structure needs to reflect the structure of MyInstanceData
				// It must also be of a form the shader expects, that is same type (glm::vec3 maps to glsl vec3)
				// and same location (6 in this example)
				std::vector<Core::VertexAttribute> instance_structure {
					Core::VaoArg<glm::vec3>(6),
					// Fill in or replace...
				};

#ifdef __APPLE__ // FIXME: Until OS X supports OpenGL 4.2
				instanced_layout.for_buffer(instance_allocation).use_as(GL_ARRAY_BUFFER)
					.bind_interleaved(instance_allocation.offset, 1, instance_structure);
#else
				instanced_layout.for_buffer(instance_allocation).use_as(GL_ARRAY_BUFFER)
					.bind_interleaved(0, 1, instance_structure);
#endif

				// Get a copy of the original render command and update it
				Core::RenderCommand instanced_render_command = original_mesh->get_render_command();
				instanced_render_command.set_instances(instance_allocation, instance_collection);

				std::cout << "Are we instanced? " << instanced_layout.is_instanced() << std::endl;
				std::cout << "Num instances: " << instanced_render_command.instance_count << std::endl;

				Framework::MeshPtr instanced_mesh = std::make_shared<Framework::Mesh>(instanced_render_command,
					instanced_layout, app->get_vao_manager(), clan::string_format("%1%2", original_mesh->get_name(), "_instanced"));

				// Then we override the original mesh in our renderable
				renderable->set_mesh(instanced_mesh);
			}
		}
	}

	// Set up the projection for the camera
	if (camera_entity->has_component<Framework::Camera>()) {
		auto camera = camera_entity->get_component<Framework::Camera>();
		camera->set_projection(app->get_resolution());
	}

	Framework::TransformPtr camera_transform;
	if (camera_entity->has_component<Framework::Transform>()) {
		camera_transform = camera_entity->get_component<Framework::Transform>();
	}

	Framework::RenderablePtr terrain_renderable;
	if (terrain->has_component<Framework::Renderable>()) {
		terrain_renderable = terrain->get_component<Framework::Renderable>();
		terrain_renderable->get_material()->get<int>(GM_PROPERTY_PATCH_VERTICES) = 4;
	}
	
	// Hide the cursor since we have an FPS Camera
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

		if (terrain_renderable->get_material()->has_property("camera_position")) {
			terrain_renderable->get_material()->get<glm::vec3>("camera_position") = camera_transform->get_position();
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
