#include "GM/Application/Main.h"

#include "GM/Core/Utilities/ShaderFactory.h"

#include "GM/Framework/Entity.h"

#include "GM/Framework/EntityManager.h"
#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/MaterialManager.h"
#include "GM/Framework/Managers/MeshManager.h"
#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Utilities/Mesh.h"
#include <GM/Framework/Utilities/Tools.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

#define POSITION 0

using namespace GM;
using namespace Application;

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
	}
private:
	MainPtr app;
	clan::SlotContainer slots;
};

void create_triangle_mesh(const MainPtr &app) {
	std::vector<glm::vec3> vertices{
			{ -1.0f, -1.0f, 0.0f },
			{ 1.0f, -1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
	};

	auto buffer_allocation = app->get_buffer_manager()->allocate_and_upload(vertices);

	Core::VaoLayout vao_layout;
	vao_layout
		.for_buffer(buffer_allocation)
		.use_as(GL_ARRAY_BUFFER)
		.bind<glm::vec3>(POSITION)
		;

	Core::RenderCommand render_command(false, vertices.size(), 0, buffer_allocation.offset / sizeof(glm::vec3));

	auto mesh = std::make_shared<Framework::Mesh>("triangle", render_command, vao_layout, app->get_vao_manager());
	app->get_mesh_manager()->add(mesh->get_name(), mesh);
}

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);
	auto entity_manager = app->get_entity_manager();
	auto component_serializer = std::make_shared<MyComponentSerializer>(app);

	auto json_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/json/samples/triangle");
	auto glsl_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), "resources/glsl/samples/triangle");

	entity_manager->add_templates(json_path + "/entity_templates.json");
	app->get_material_manager()->add_templates(json_path + "/material_templates.json");
	app->get_shader_manager()->add_templates(json_path + "/shader_templates.json");
	app->get_shader_manager()->set_glsl_path(glsl_path);

	create_triangle_mesh(app);

	auto entity = entity_manager->create_entity("entity");
	entity_manager->apply("triangle", entity);

	unsigned int num_renders = 1*30;
	unsigned int count = 0;

	auto update_slot = app->on_update().connect([&](float /*value*/) mutable {
		++count;
		if (count >= num_renders)
		{
			app->stop_running();
		}
	});

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
