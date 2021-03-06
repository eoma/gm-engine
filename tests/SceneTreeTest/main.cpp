#include <cstdlib>
#include <GM/Framework/DefinitionsPropertyNames.h>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/SceneSystem.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>

#include <memory>

using namespace GM;
using namespace Framework;

bool mainTest() {

	clan::SlotContainer slots;

	auto entity_manager = std::make_shared<EntityManager>();
	auto scene_system = std::make_shared<SceneSystem>();

	auto parent = entity_manager->create_entity("parent");
	auto child1 = entity_manager->create_entity("child1");
	auto child2 = entity_manager->create_entity("child2");

	auto parent_transform = parent->create_component<Transform>(scene_system);
	auto child1_transform = child1->create_component<Transform>(scene_system);
	auto child2_transform = child2->create_component<Transform>(scene_system);

	parent_transform->add_child(child1_transform);
	parent_transform->add_child(child2_transform);

	bool child1changed = false;
	bool child2changed = false;

	// Add world matrix property value listeners on the children
	slots.connect(
		child1->get<glm::mat4>(GM_PROPERTY_WORLD_MATRIX).value_changed(),
		[&](const glm::mat4 &/*o*/, const glm::mat4 &/*n*/) mutable { child1changed = true; }
	);

	slots.connect(
		child2->get<glm::mat4>(GM_PROPERTY_WORLD_MATRIX).value_changed(),
		[&](const glm::mat4 &/*o*/, const glm::mat4 &/*n*/) mutable { child2changed = true; }
	);

	// Only move parent transform
	parent_transform->translate(glm::vec3(1,1,1));

	scene_system->prepare();

	if (!child1changed || !child2changed) {
		throw clan::Exception("Child 1 and child 2 should have been changed when preparing scene graph after parent has changed.");
	}

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
