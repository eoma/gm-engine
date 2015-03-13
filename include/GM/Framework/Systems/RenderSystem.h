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

class CameraMatricesUbo; typedef std::shared_ptr<CameraMatricesUbo> CameraMatricesUboPtr;
class LightListUbo; typedef std::shared_ptr<LightListUbo> LightListUboPtr;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	void add_renderable(IRenderable *renderable);
	void remove_renderable(IRenderable *renderable);

	void add_camera(Camera *camera);
	void remove_camera(Camera *camera);
	void sort_cameras();

	void add_light(Light *light);
	void remove_light(Light *light);

	/**
	 * Perform a render
	 */
	void render();

	/**
	 * Render elements with shaders for a specific pass and elements from certain layers.
	 *
	 * @param camera the current active camera
	 * @param render_pass_name the wanted render pass
	 * @param accepted_layers will be &ed with cameras layers
	 */
	void pass(Camera &camera, const std::string &render_pass_name, unsigned int accepted_layers);

	/**
	 * Set current render area
	 */
	void resize(int width, int height);

	/**
	 * Get a list of renderables for a given layer.
	 *
	 * The method will throw an std::out_of_range exception if bucket_index > 31
	 */
	const std::vector<IRenderable*> &get_bucket(const unsigned int bucket_index);

	/**
	 * Get all registered cameras
	 */
	const std::vector<Camera*> &get_cameras() { return cameras; }

	/**
	 * Get all registered cameras in a render layer
	 */
	const std::vector<Camera*> &get_cameras(const unsigned int layer_index);

	/**
	 * Return a list of all registered lights
	 */
	const std::vector<Light *> &get_lights() const { return lights; }

	void set_camera_matrices_ubo(const CameraMatricesUboPtr &new_camera_matrices_ubo) { camera_matrices_ubo = new_camera_matrices_ubo; }

	void set_light_list_ubo(const LightListUboPtr &new_light_list_ubo) { light_list_ubo = new_light_list_ubo; }

public:

	/**
	 * Convert a single unsigned 32-bit integer (n = 2^a + 2^b + ..., a < b < ...)
	 * in to a list of exponents of base 2 (n is converted to [a, b, ...])
	 */
	static std::vector<unsigned int> bit_index_maker(const unsigned int bits);

private:
	/**
	 * Update camera and light uniform buffer objects
	 */
	void prepare_ubos(const Camera &active_camera);

private:
	/**
	 * Position is interpreted as layer. Position 0 is interpreted as layer 1,
	 * position 1 is layer 2, and so forth.
	 */
	std::array<std::vector<IRenderable*>, std::numeric_limits<unsigned int>::digits> buckets;

	/**
	 * Follows almost same structure as the buckets structure.
	 * Position is determined by layer. The order in the layer's
	 * vector is depth sorted.
	 */
	std::array<std::vector<Camera*>, std::numeric_limits<unsigned int>::digits> cameras_in_layers;

	/**
	 * Depth sorted cameras. Determines the render order.
	 */
	std::vector<Camera*> cameras;

	/**
	 * We track lights in the scene in a special list, since
	 * lights affect all geometry in scene.
	 */
	std::vector<Light*> lights;

	/**
	 * An object that manages updating of a uniform buffer block containing lights
	 */
	LightListUboPtr light_list_ubo;

	/**
	 * An object responsible for updating a uniform buffer block containing common camera matrices
	 */
	CameraMatricesUboPtr camera_matrices_ubo;

};

typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

} // namespace Framework
} // namespace GM
