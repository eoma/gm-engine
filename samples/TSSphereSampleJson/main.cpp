#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"
#include "GM/Framework/Utilities/Material.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Samples/SamplesComponentSerializer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

using namespace GM;
using namespace Application;

bool mainTest() {
	auto app = Main::create_with_gl_version("TSSPhereSampleJson", 4, 1);
    app->set_resolution(1024, 768);

    auto entity_manager = app->get_entity_manager();
    auto samples_component_serializer = std::make_shared<Samples::SamplesComponentSerializer>(app);

	// Set up resource data path locations
	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/ts-sphere");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/ts-sphere");

	// Set up resource data
	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);
    
	// Create our entities
    auto camera = entity_manager->create_entity("camera");
	auto icosahedron = entity_manager->create_entity("icosahedron");

	// Apply the entity template "cube", defined in entity_templates.json
    entity_manager->apply("fps_camera", camera);
	entity_manager->apply("icosahedron", icosahedron);

    // Set up the projection for the camera
    if (camera->has_component<Framework::Camera>()) {
        camera->get_component<Framework::Camera>()->set_projection(app->get_resolution());
    }
    
    app->hide_cursor();
    
	// Set some run time limits
	float max_run_time = 100.f;
	float run_time = 0.f;
    
    auto keyboard_slot = app->sign_keyboard().connect([&](int key, int scancode, int action, int mods) {
       if (mods == GLFW_MOD_SHIFT)
       {
           auto tessLevelOuter = icosahedron->get<int>("TessLevelOuter");
           if (action == GLFW_RELEASE && key == GLFW_KEY_UP)
           {
               tessLevelOuter++;
               std::cout << "tessLevelOuter = " << tessLevelOuter << std::endl;
           }
           else if (action == GLFW_RELEASE && key == GLFW_KEY_DOWN)
           {
               if (tessLevelOuter > 1)
               {
                   tessLevelOuter--;
               }
               std::cout << "tessLevelOuter = " << tessLevelOuter << std::endl;
           }
       }
       else
       {
           auto tessLevelInner = icosahedron->get<int>("TessLevelInner");
           if (action == GLFW_RELEASE && key == GLFW_KEY_UP)
           {
               tessLevelInner++;
               std::cout << "tessLevelInner = " << tessLevelInner << std::endl;
           }
           else if (action == GLFW_RELEASE && key == GLFW_KEY_DOWN)
           {
               if (tessLevelInner > 1)
               {
                   tessLevelInner--;
                   std::cout << "tessLevelInner = " << tessLevelInner << std::endl;
               }
           }
       }
    });

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
