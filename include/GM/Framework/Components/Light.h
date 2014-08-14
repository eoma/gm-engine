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

class Light : public Component<Light> {
public:
	Light(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name = std::string());
	virtual ~Light();

	std::string get_type() const override { return get_static_type(); };

	glm::vec3 get_position_in_viewspace(Camera *camera) const;
	float get_radius() const { return radius_property; }
	glm::vec3 get_material_color_diffuse() const { return material_color_diffuse_property; }
	glm::vec3 get_material_color_specular() const { return material_color_specular_property; }
	glm::vec3 get_material_color_ambient() const { return material_color_ambient_property; }

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
};

} // namespace Framework
} // namespace GM
