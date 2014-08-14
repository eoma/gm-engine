#include <cstdlib>

#include <GM/Framework/Totem/Totem.h>
#include <GM/Framework/DefinitionsPropertyNames.h>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/RenderSystem.h>
#include <GM/Framework/Systems/SceneSystem.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>
#include <GM/Framework/Managers/ShaderManager.h>
#include <GM/Framework/Managers/TextureManager.h>
#include <GM/Framework/Managers/MaterialManager.h>
#include <GM/Framework/Managers/BufferManager.h>
#include <GM/Framework/Managers/VaoManager.h>
#include <GM/Framework/Managers/MeshManager.h>
#include <GM/Framework/Utilities/ComponentSerializer.h>
#include <GM/Framework/Utilities/Tools.h>
#include <GM/Framework/IO/SoilImageIO.h>
#include <GM/Framework/IO/AssimpMeshIO.h>

#include <memory>

using namespace GM;
using namespace Framework;

class MyComponentSerializer {
public:

	MyComponentSerializer(const EntityManagerPtr &entity_manager, const SceneSystemPtr &scene_system, const RenderSystemPtr &render_system, const MaterialManagerPtr& material_manager, const MeshManagerPtr mesh_manager)
		: scene_system(scene_system), render_system(render_system)
		, material_manager(material_manager), mesh_manager(mesh_manager)
	{
		slots.connect(
			entity_manager->register_component_serializer_signal(),
			this, &MyComponentSerializer::create_and_add_component);
	}

	void create_and_add_component(const EntityPtr &owner, const std::string &type, const std::string &/*name*/) {
		if (type == Transform::get_static_type()) {
			owner->create_component<Transform>(scene_system);
		}
		else if (type == Renderable::get_static_type()) {
			owner->create_component<Renderable>(render_system, material_manager, mesh_manager);
		}
		//etc
	}

private:
	SceneSystemPtr scene_system;
	RenderSystemPtr render_system;

	MaterialManagerPtr material_manager;
	MeshManagerPtr mesh_manager;

	clan::SlotContainer slots;
};

void on_component_added(std::shared_ptr<IComponent<>> component) {
	std::string name = component->get_name();
	if (name.empty())
		name = component->get_type();

	std::cout << "Component " << name << " was added to Entity " << component->get_owner()->get_name() << "!" << std::endl;
}

void on_position_changed(const glm::vec3 &old_value, const glm::vec3 &new_value) {
	std::cout << "Position property changed from [" +
		clan::StringHelp::float_to_text(old_value.x, 1) + ", " +
		clan::StringHelp::float_to_text(old_value.y, 1) + ", " +
		clan::StringHelp::float_to_text(old_value.z, 1) + "] to [" +
		clan::StringHelp::float_to_text(new_value.x, 1) + ", " +
		clan::StringHelp::float_to_text(new_value.y, 1) + ", " +
		clan::StringHelp::float_to_text(new_value.z, 1) + "]."
		<< std::endl;
}

bool mainTest() {

	clan::SlotContainer slots;
	
	auto render_system = std::make_shared<RenderSystem>();
	auto scene_system = std::make_shared<SceneSystem>();
	auto entity_manager = std::make_shared<EntityManager>();
	auto shader_manager = std::make_shared<ShaderManager>();
	auto texture_manager = std::make_shared<TextureManager>(std::make_shared<Framework::SoilImageIO>());
	auto buffer_manager = std::make_shared<BufferManager>();
	auto vao_manager = std::make_shared<VaoManager>();
	auto material_manager = std::make_shared<MaterialManager>(shader_manager, texture_manager);
	auto mesh_manager = std::make_shared<MeshManager>(buffer_manager, vao_manager, std::make_shared<Framework::AssimpMeshIO>());

	auto my_component_serializer = std::make_shared<MyComponentSerializer>(entity_manager, scene_system, render_system, material_manager, mesh_manager);

	auto path = clan::System::get_exe_path();
	std::cout << "Base path: " << path << std::endl;

	std::string wanted_directory = "resources/json/tests";

	path = find_path_in_hierarchy(path, wanted_directory);
	std::cout << "Resource path: " << path << std::endl;

	entity_manager->add_templates(path + "/entity_templates.json");

	auto entity1 = entity_manager->create_entity("One");
	auto entity2 = entity_manager->create_entity("Two");
	auto entity3 = entity_manager->create_entity("Three");

	slots.connect(entity1->component_added(), &on_component_added);
	slots.connect(entity2->component_added(), &on_component_added);
	slots.connect(entity3->component_added(), &on_component_added);

	slots.connect(entity1->add<glm::vec3>(GM_PROPERTY_POSITION, glm::vec3(0, 0, 0)).value_changed(), [&](const glm::vec3 &old_value, const glm::vec3 &new_value) mutable {
		std::cout << "Entity " + entity1->get_name() << "'s "; 
		on_position_changed(old_value, new_value);
	});

	slots.connect(entity2->add<glm::vec3>(GM_PROPERTY_POSITION, glm::vec3(0, 0, 0)).value_changed(), [&](const glm::vec3 &old_value, const glm::vec3 &new_value) mutable {
		std::cout << "Entity " + entity2->get_name() << "'s ";
		on_position_changed(old_value, new_value);
	});

	slots.connect(entity3->add<glm::vec3>(GM_PROPERTY_POSITION, glm::vec3(0, 0, 0)).value_changed(), [&](const glm::vec3 &old_value, const glm::vec3 &new_value) mutable {
		std::cout << "Entity " + entity3->get_name() << "'s ";
		on_position_changed(old_value, new_value);
	});

	entity_manager->apply("Test", entity1);
	entity_manager->apply("Test2", entity2);
	entity_manager->apply("Test3", entity3);

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
