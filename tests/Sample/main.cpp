#include <cstdlib>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/SceneManager.h>
#include <GM/Framework/Components/Transform.h>

bool mainTest() {

	auto entity_manager = GM::Framework::EntityManagerPtr(new GM::Framework::EntityManager());
	auto scene_manager = GM::Framework::SceneManagerPtr(new GM::Framework::SceneManager());

	auto root_entity = entity_manager->create_entity("Root");
	auto root_transform = root_entity->addComponent<GM::Framework::Transform>(GM::Framework::TransformPtr(new GM::Framework::Transform(root_entity, scene_manager)));

	auto child_entity = entity_manager->create_entity("Child");
	auto child_transform = child_entity->addComponent<GM::Framework::Transform>(GM::Framework::TransformPtr(new GM::Framework::Transform(child_entity, scene_manager)));

	root_transform->add_child(child_transform);

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
