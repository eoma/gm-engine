#pragma once

#include <array>
#include <limits>
#include <memory>
#include <vector>


namespace GM {
namespace Framework {

class Camera;
class Light;
class IRenderable; typedef std::shared_ptr<IRenderable> IRenderablePtr;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	void add_renderable(IRenderable *renderable);
	void remove_renderable(IRenderable *renderable);

	void add_camera(Camera *camera);
	void remove_camera(Camera *camera);

	void add_light(Light *light);
	void remove_light(Light *light);

	void render();

	void resize(int width, int height);

	// The methods will throw an std::out_of_range exception if bcuket_index > 31
	const std::vector<IRenderable*> &get_bucket(const unsigned int bucket_index);
	const std::vector<Camera*> &get_cameras(const unsigned int layer_index);

	const std::vector<Light *> &get_lights() const { return lights; }

public:

	// Should this be moved to a utility class?
	static std::vector<unsigned int> bit_index_maker(const unsigned int bits);

private:
	// Position is interpreted as layer. Position 0 is interpreted as layer 1,
	// position 1 is layer 2, and so forth.
	std::array<std::vector<IRenderable*>, std::numeric_limits<unsigned int>::digits> buckets;

	// Follows almost same structure as the buckets structure.
	// Position is determined by layer. The order in the layer's
	//  vector is depth sorted.
	std::array<std::vector<Camera*>, std::numeric_limits<unsigned int>::digits> cameras_in_layers;

	// We track lights in the scene in a special list, since
	// lights affect all geometry in scene.
	std::vector<Light*> lights;
};

typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

} // namespace Framework
} // namespace GM
