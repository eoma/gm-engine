#include <cstdlib>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/SceneManager.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>

#include <memory>

using namespace GM;
using namespace Framework;

void on_component_added(std::shared_ptr<Totem::IComponent<>> component) {
	std::string owner_name = "Unknown";
	if (Totem::IComponent<>::isType<Transform>(component)) {
		auto transform = std::static_pointer_cast<Transform>(component);
		owner_name = transform->get_owner()->get_name();
	}
	else if (Totem::IComponent<>::isType<Renderable>(component)) {
		auto renderable = std::static_pointer_cast<Renderable>(component);
		owner_name = renderable->get_owner()->get_name();
	}
	std::cout << "Component " << component->getName() << " was added to " << owner_name << "!" << std::endl;
}

void on_component_removed(std::shared_ptr<Totem::IComponent<>> component) {
	std::string owner_name = "Unknown";
	if (Totem::IComponent<>::isType<Transform>(component)) {
		auto transform = std::static_pointer_cast<Transform>(component);
		owner_name = transform->get_owner()->get_name();
	}
	else if (Totem::IComponent<>::isType<Renderable>(component)) {
		auto renderable = std::static_pointer_cast<Renderable>(component);
		owner_name = renderable->get_owner()->get_name();
	}
	if (owner_name.empty())
		std::cout << "Component " << component->getName() << " was removed from its entity!" << std::endl;
	else
		std::cout << "Component " << component->getName() << " was removed from " << owner_name << "!" << std::endl;
}

void on_transform_child_added(const Transform * const parent, const Transform * const child) {
	std::cout << "Transform " << child->get_owner()->get_name() << " was added as child to parent transform " << parent->get_owner()->get_name() << std::endl;
}

bool mainTest() {

	typedef clan::Callback<void(const Transform * const, const Transform * const)> CallbackParentChild;

	auto entity_manager = std::make_shared<EntityManager>();
	auto scene_manager = std::make_shared<SceneManager>();

	auto root_entity = entity_manager->create_entity("Root");
	root_entity->componentAdded().connect(clan::Callback<void(std::shared_ptr<Totem::IComponent<>>)>(&on_component_added));
	root_entity->componentRemoved().connect(clan::Callback<void(std::shared_ptr<Totem::IComponent<>>)>(&on_component_removed));
	auto root_transform = root_entity->addComponent<Transform>(std::make_shared<Transform>(root_entity, scene_manager, Transform::get_static_type()));
	root_transform->get_child_added_signal().connect(CallbackParentChild(&on_transform_child_added));

	auto child_entity = entity_manager->create_entity("Child");
	child_entity->componentAdded().connect(clan::Callback<void(std::shared_ptr<Totem::IComponent<>>)>(&on_component_added));
	child_entity->componentRemoved().connect(clan::Callback<void(std::shared_ptr<Totem::IComponent<>>)>(&on_component_removed));
	auto child_transform = child_entity->addComponent<Transform>(std::make_shared<Transform>(child_entity, scene_manager, Transform::get_static_type()));
	child_transform->get_child_added_signal().connect(CallbackParentChild(&on_transform_child_added));

	root_transform->add_child(child_transform);

	scene_manager.reset();
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
