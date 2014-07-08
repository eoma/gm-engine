#include <cstdlib>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/SceneSystem.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>

#include <memory>

using namespace GM;
using namespace Framework;

void on_component_added(std::shared_ptr<IComponent<>> component) {
	std::cout << "Component " << component->get_name() << " was added to " << component->get_owner()->get_name() << "!" << std::endl;
}

void on_component_removed(std::shared_ptr<IComponent<>> component) {
	std::cout << "Component " << component->get_name() << " was removed from " << component->get_owner()->get_name() << "!" << std::endl;
}

void on_transform_child_added(const Transform * const parent, const Transform * const child) {
	std::cout << "Transform " << child->get_owner()->get_name() << " was added as child to parent transform " << parent->get_owner()->get_name() << std::endl;
}

bool mainTest() {

	clan::SlotContainer slots;

	auto entity_manager = std::make_shared<EntityManager>();
	auto scene_system = std::make_shared<SceneSystem>();

	auto root_entity = entity_manager->create_entity("Root");
	slots.connect(root_entity->component_added(), &on_component_added);
	slots.connect(root_entity->component_removed(), &on_component_removed);
	
	auto root_transform = root_entity->add_component<Transform>(std::make_shared<Transform>(root_entity, scene_system, Transform::get_static_type()));
	slots.connect(root_transform->get_child_added_signal(), &on_transform_child_added);

	auto child_entity = entity_manager->create_entity("Child");
	slots.connect(child_entity->component_added(), &on_component_added);
	slots.connect(child_entity->component_removed(), &on_component_removed);

	auto child_transform = child_entity->add_component<Transform>(std::make_shared<Transform>(child_entity, scene_system, Transform::get_static_type()));
	slots.connect(child_transform->get_child_added_signal(), &on_transform_child_added);

	root_transform->add_child(child_transform);

	scene_system.reset();
	entity_manager.reset();

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
