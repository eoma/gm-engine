#include <cstdlib>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>

#include <memory>

using namespace GM;
using namespace Framework;

bool mainTest() {

	auto entity_manager = std::make_shared<EntityManager>();

	auto entity1 = entity_manager->create_entity("one");
	auto entity2 = entity_manager->create_entity("two");

	if (entity1->get_id() == entity2->get_id()) {
		throw clan::Exception("Entity id's must be different!");
	}

	auto removedEntity1 = entity_manager->remove_entity(entity1->get_id(), true);

	if (removedEntity1 == nullptr) {
		throw clan::Exception("The attempt at removing an entity should have been successful.");
	}

	removedEntity1 = entity_manager->remove_entity(entity1->get_id(), true);

	if (removedEntity1 != nullptr) {
		throw clan::Exception("The attempt at double removing an entity should have been unsuccessful");
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
