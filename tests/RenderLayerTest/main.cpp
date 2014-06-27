#include <cstdlib>
#include <GM/Framework/EntityManager.h>
#include <GM/Framework/Entity.h>
#include <GM/Framework/Systems/SceneManager.h>
#include <GM/Framework/Systems/RenderSystem.h>
#include <GM/Framework/Components/Transform.h>
#include <GM/Framework/Components/Renderable.h>

#include <memory>

using namespace GM;
using namespace Framework;

class RenderableTest : public Component<RenderableTest>, public IRenderable {
public:
	RenderableTest(const EntityPtr &owner, const RenderSystemPtr &render_system, const unsigned int render_layers = 1): Component(owner, std::string()), render_system(render_system), render_layers(render_layers) {
	render_system->add_renderable(this);
};
	virtual ~RenderableTest() {
		render_system->remove_renderable(this);
	};

	virtual std::string get_type() const override { return "RenderableTest"; };

	virtual glm::mat4 get_world_matrix() const override { return glm::mat4(); };	
	virtual glm::mat4 get_object_matrix() const override { return glm::mat4(); };

	virtual bool is_culled() const override { return false; };
	virtual bool is_invisible() const override { return false; };

	virtual void render(Camera * /*camera*/) override {};

	virtual unsigned int get_render_layers() const override { return render_layers; };

private:
	RenderSystemPtr render_system;

	unsigned int render_layers;
};

bool mainTest() {

	typedef clan::Callback<void(const Transform * const, const Transform * const)> CallbackParentChild;

	auto entity_manager = std::make_shared<EntityManager>();
	auto render_system = std::make_shared<RenderSystem>();

	auto entity1 = entity_manager->create_entity("one");
	auto entity2 = entity_manager->create_entity("two");

	auto renderable1 = entity1->add_component(std::make_shared<RenderableTest>(entity1, render_system, (1<<0) + (1<<2)));

	if (render_system->get_bucket(0).size() != 1 || render_system->get_bucket(2).size() != 1) {
		throw std::runtime_error("A renderable registered in layers 0 and 2 is not present in one of these");
	}

	auto renderable2 = entity2->add_component(std::make_shared<Renderable>(entity2, render_system, 1<<1));

	if (!(render_system->get_bucket(1).size() == 1 && render_system->get_bucket(1)[0] == renderable2.get())) {
		throw std::runtime_error("A renderable registered in layer 1 (for entity2) is not present");
	}

	// Clean up
	entity1->remove_component<RenderableTest>();
	renderable1 = nullptr;

	entity2->remove_component<Renderable>();
	renderable2 = nullptr;

	// Check that the render system is indeed empty
	for (int i = 0; i < std::numeric_limits<unsigned int>::digits; ++i) {
		if (render_system->get_bucket(i).size() != 0) {
			throw std::runtime_error("RenderSystem should not contain any renderables when the only renderable has been removed");
		}
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
