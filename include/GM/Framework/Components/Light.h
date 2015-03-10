#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM {
namespace Framework {

class Light; typedef std::shared_ptr<Light> LightPtr;
class Entity; typedef std::shared_ptr<Entity> EntityPtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
class Camera;

/**
 * A component representing a light in a scene.
 */
class Light : public Component<Light> {
public:
	Light(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name = std::string());
	virtual ~Light();

	std::string get_type() const override { return get_static_type(); };

	/**
	 * Get position in the current world space
	 *
	 * @return position in worldspace
	 */
	glm::vec3 get_position_in_worldspace() const;

	/**
	 * Get position in the view space of a camera.
	 *
	 * @param camera the relevant camera.
	 * @return position in the camera's view space.
	 */
	glm::vec3 get_position_in_viewspace(const Camera &camera) const;

	/**
	 * Set the radius of the light. Determines the attenuation of the light.
	 * The larger the value, the less attenuation.
	 *
	 * @return the radius determining the attenuation
	 */
	float get_radius() const { return radius_property; }

	/**
	 * Get the diffuse color.
	 *
	 * @return diffuse color
	 */
	glm::vec3 get_material_color_diffuse() const { return material_color_diffuse_property; }

	/**
	 * Get specular color
	 *
	 * @return specular color
	 */
	glm::vec3 get_material_color_specular() const { return material_color_specular_property; }

	/**
	 * Get ambient color
	 *
	 * @return ambient color
	 */
	glm::vec3 get_material_color_ambient() const { return material_color_ambient_property; }

	/**
	 * Check if this light is activated.
	 */
	bool get_activated() const { return activated_property; }

	/**
	 * Check if this light is a shadow caster
	 */
	bool is_shadow_caster() const { return shadow_caster_property; }

	/**
	 * Check if this light is supposed to be active
	 */
	bool is_active() const { return activated_property; }

public:
	static std::string get_static_type() { return GM_COMPONENT_LIGHT; };

private:
	clan::SlotContainer slots;

	RenderSystemPtr render_system;

	Property<glm::mat4> world_matrix_property;
	Property<float> radius_property;
	Property<glm::vec3> material_color_diffuse_property;
	Property<glm::vec3> material_color_specular_property;
	Property<glm::vec3> material_color_ambient_property;
	Property<bool> activated_property;
	Property<bool> shadow_caster_property;
};

} // namespace Framework
} // namespace GM
