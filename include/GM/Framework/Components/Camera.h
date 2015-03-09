#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"
#include "../RenderLayers.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM {

namespace Core {
class Texture; typedef std::shared_ptr<Texture> TexturePtr;
class ReadWriteTexture; typedef std::shared_ptr<ReadWriteTexture> ReadWriteTexturePtr;
}

namespace Framework {

class Camera; typedef std::shared_ptr<Camera> CameraPtr;
class Entity; typedef std::shared_ptr<Entity> EntityPtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
class TextureManager; typedef std::shared_ptr<TextureManager> TextureManagerPtr;
class IRenderPassComponent;
class FinalPass; typedef std::shared_ptr<FinalPass> FinalPassPtr;

/**
 * Represents a camera in a scene.
 */
class Camera : public Component<Camera> {
public:
	Camera(const EntityPtr &owner, const RenderSystemPtr &render_system, const TextureManagerPtr &texture_manager, unsigned int render_layers = RenderLayers::MESH_OPAQUE, int depth = 0, const std::string &name = std::string());
	virtual ~Camera();

	std::string get_type() const override { return get_static_type(); };
	static std::string get_static_type() { return GM_COMPONENT_CAMERA; };

	/**
	 * Initialize this component. This method will primarily set up the render pass sequence.
	 * It should only run when the necessary whatever necessary components and properties
	 * has been added by the template manager or user.
	 */
	void initialize() override;

	/**
	 * Fetch the generated 
	 *
	 * @return list of render passes.
	 */
	const std::vector<IRenderPassComponent*> &get_render_pass_sequence() const { return pass_sequence; }

	/**
	 * Goes through all the components added to the entity and checks if they implement
	 * the IRenderPassComponent interface. If so, it will add to the render pass sequence.
	 */
	void make_render_pass_sequence();

	/**
	 * What layer should this camera be used for?
	 */
	unsigned int get_render_layers() const { return render_layers; };

	/**
	 * Will trigger reregistration of camera.
	 */
	void set_render_layers(unsigned int render_layers);

	/**
	 * At what order is this camera to be used in a render.
	 * Lower number means earlier.
	 */
	int get_depth() const { return depth; };

	/**
	 * Will trigger reregistration of camera.
	 */
	void set_depth(int depth);

	bool is_view_matrix_dirty() const { return view_matrix_property.is_dirty(); };
	const glm::mat4 &get_view_matrix() const { return view_matrix_property; };

	bool is_projection_matrix_dirty() const { return projection_matrix_property.is_dirty(); };
	const glm::mat4 &get_projection_matrix() const { return projection_matrix_property; };
	void set_projection(float fov, float width, float height, float near_clipping, float far_clipping);
	void set_projection(const glm::uvec2 &resolution);
	void set_projection(float width, float height);

	float get_max_vertical_angle() const { return max_vertical_angle_property; }

	/**
	 * Reset the dirty flag on the view and projection matrices
	 */
	void clear_dirty();

	/**
	 * Get the final render texture produced by this camera's render pass sequence
	 */
	Core::TexturePtr get_render_texture() const;

	/**
	 * Should this camera render to screen?
	 */
	void set_render_to_screen(bool status) { render_to_screen = status; }
	bool get_render_to_screen() const { return render_to_screen; }

private:
	void recalculate_view_matrix(const glm::mat4 &old_world, const glm::mat4 &new_world);

private:
	clan::SlotContainer slots;

	RenderSystemPtr render_system;

	// What render layer is this camera supposed to follow?
	unsigned int render_layers;

	// Depth determines the order in the render layer, smallest deth gets rendered first, and so on.
	int depth;

	unsigned int current_width;
	unsigned int current_height;

	Property<glm::mat4> projection_matrix_property;
	Property<glm::mat4> view_matrix_property;
	Property<glm::mat4> world_matrix_property;

	Property<float> fov_property;
	Property<float> near_clipping_property;
	Property<float> far_clipping_property;
	Property<float> max_vertical_angle_property;

	Property<bool> render_to_screen;

	// Render pass relevant variables
	std::vector<IRenderPassComponent*> pass_sequence;

	// Render texture for use in texture passes
	Core::ReadWriteTexturePtr render_texture;

	FinalPassPtr final_pass;
};

} // namespace Framework
} // namespace GM
