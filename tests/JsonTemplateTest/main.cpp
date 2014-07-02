#include <cstdlib>

#include <GM/Framework/Totem/Totem.h>
#include <GM/Framework/DefinitionsPropertyNames.h>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/RenderSystem.h>
#include <GM/Framework/Systems/SceneManager.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>
#include <GM/Framework/Utilities/TemplateManager.h>
#include <GM/Framework/Utilities/ComponentSerializer.h>

#include <memory>

using namespace GM;
using namespace Framework;

class MyComponentSerializer {
public:

	MyComponentSerializer(const ComponentSerializerPtr &component_serializer, const SceneManagerPtr &scene_manager, const RenderSystemPtr &render_system)
		: scene_manager(scene_manager), render_system(render_system)
	{
		component_serializer->sig_create_component.connect(
			clan::Callback<void(const EntityPtr &, const std::string &, const std::string &)>(
			this, &MyComponentSerializer::create_and_add_component));
	}

	void create_and_add_component(const EntityPtr &owner, const std::string &type, const std::string &/*name*/) {
		if (type == Transform::get_static_type()) {
			owner->create_component<Transform>(scene_manager);
		}
		else if (type == Renderable::get_static_type()) {
			owner->create_component<Renderable>(render_system);
		}
		//etc
	}

private:
	SceneManagerPtr scene_manager;
	RenderSystemPtr render_system;
};

std::string trim_path(std::string path, const std::string &sign, unsigned int num_trims) {
	for (unsigned int i = 0; i < num_trims; i++) {
		path = path.substr(0, path.find_last_of(sign));
	}
	return path + sign;
}

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

	auto entity_manager = std::make_shared<EntityManager>();
	auto render_system = std::make_shared<RenderSystem>();
	auto scene_manager = std::make_shared<SceneManager>();
	auto component_serializer = std::make_shared<ComponentSerializer>();
	auto template_manager = std::make_shared<TemplateManager>(component_serializer);

	auto my_component_serializer = std::make_shared<MyComponentSerializer>(component_serializer, scene_manager, render_system);
	

	auto path = clan::System::get_exe_path();
	std::cout << "Base path: " << path << std::endl;

#if WIN32
	path = trim_path(path, "\\", 4);
	std::cout << "Trimmed path: " << path << std::endl;
#else
	//TODO: Set this up for Linux/OsX?
#endif

	path += "resources/json/tests/";
	std::cout << "Resource path: " << path << std::endl;

	template_manager->add_templates(path + "test_entity_template1.js");

	auto entity1 = entity_manager->create_entity("One");
	auto entity2 = entity_manager->create_entity("Two");
	auto entity3 = entity_manager->create_entity("Three");

	entity1->component_added().connect(clan::Callback<void(std::shared_ptr<IComponent<>>)>(&on_component_added));
	entity2->component_added().connect(clan::Callback<void(std::shared_ptr<IComponent<>>)>(&on_component_added));
	entity3->component_added().connect(clan::Callback<void(std::shared_ptr<IComponent<>>)>(&on_component_added));

	entity1->add<glm::vec3>(PROPERTY_POSITION, glm::vec3(0,0,0)).value_changed().connect(
		std::function<void(const glm::vec3&, const glm::vec3&)>([&](const glm::vec3 &old_value, const glm::vec3 &new_value) mutable {
		std::cout << "Entity " + entity1->get_name() << "'s "; 
		on_position_changed(old_value, new_value);
	}));

	entity2->add<glm::vec3>(PROPERTY_POSITION, glm::vec3(0, 0, 0)).value_changed().connect(
		std::function<void(const glm::vec3&, const glm::vec3&)>([&](const glm::vec3 &old_value, const glm::vec3 &new_value) mutable {
		std::cout << "Entity " + entity2->get_name() << "'s ";
		on_position_changed(old_value, new_value);
	}));

	entity3->add<glm::vec3>(PROPERTY_POSITION, glm::vec3(0, 0, 0)).value_changed().connect(
		std::function<void(const glm::vec3&, const glm::vec3&)>([&](const glm::vec3 &old_value, const glm::vec3 &new_value) mutable {
		std::cout << "Entity " + entity3->get_name() << "'s ";
		on_position_changed(old_value, new_value);
	}));

	template_manager->apply("Test", entity1);
	template_manager->apply("Test2", entity2);
	template_manager->apply("Test3", entity3);

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
