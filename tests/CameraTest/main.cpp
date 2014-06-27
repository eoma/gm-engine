#include <cstdlib>

#include <GM/Framework/DefinitionsPropertyNames.h>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/SceneManager.h>
#include <GM/Framework/Systems/RenderSystem.h>
#include <GM/Framework/Components/Camera.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>

#include <memory>

using namespace GM;
using namespace Framework;

bool mainTest() {

	typedef clan::Callback<void(const Transform * const, const Transform * const)> CallbackParentChild;

	auto entity_manager = std::make_shared<EntityManager>();
	auto render_system = std::make_shared<RenderSystem>();
	auto scene_manager = std::make_shared<SceneManager>();

	auto entity1 = entity_manager->create_entity("one");
	auto entity2 = entity_manager->create_entity("two");

	auto transform1 = entity1->add_component<Transform>(std::make_shared<Transform>(entity1, scene_manager));
	auto camera1 = entity1->add_component<Camera>(std::make_shared<Camera>(entity1, render_system));

	bool changed = false;

	entity1->add<glm::mat4>(PROPERTY_VIEW_MATRIX, glm::mat4()).value_changed().connect(
		std::function<void(const glm::mat4&, const glm::mat4&)>(
			[&changed](const glm::mat4 &/*o*/, const glm::mat4 &/*n*/) mutable { changed = true; }
		)
	);

	transform1->set_position(glm::vec3(1,0,0));
	transform1->update_world_matrix();

	if (!changed) {
		throw std::runtime_error("View matrix should have been automatically updated when world matrix got updated.");
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
