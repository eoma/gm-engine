#pragma once

#include "IRenderable.h"
#include "../DefinitionsComponentNames.h"

#include "../Totem/Component.h"

#include "GM/Core/Utilities/UniformOperations.h"

#include <memory>
#include <string>

namespace GM {
namespace Framework {

// Forward declarations
class MeshManager; typedef std::shared_ptr<MeshManager> MeshManagerPtr;
class MaterialManager; typedef std::shared_ptr<MaterialManager> MaterialManagerPtr;
class Renderable; typedef std::shared_ptr<Renderable> RenderablePtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
class PropertyToUniformConnector;

class Renderable : public IRenderable, public Component<Renderable> {
public:
	Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const MaterialManagerPtr material_manager, const MeshManagerPtr mesh_ptr, unsigned int render_layers = RenderLayers::MESH_OPAQUE, const std::string &name = std::string());
	virtual ~Renderable();

	std::string get_type() const override { return get_static_type(); }

	virtual	glm::mat4 get_world_matrix() const override { return world_matrix_property; }
	virtual	glm::mat4 get_object_matrix() const override { return object_matrix_property; }

	virtual bool is_culled() const override { return culled_property; }
	virtual bool is_invisible() const override { return !visible_property || is_culled(); }

	/**
	 * Set render layer.
	 */
	void set_render_layers(unsigned int new_render_layers);

	virtual unsigned int get_render_layers() const override { return render_layers; }

	/**
	 * Set a material name. The relevant material will be fetched from the material
	 * manager.
	 *
	 * @param material_name name of a material existing in the material manager
	 */
	void set_material(const std::string& material_name) { material_name_property = material_name; }

	virtual const MaterialPtr &get_material() const override { return material; }

	/**
	 * Set mesh name, which triggers a fetching of the mesh from the mesh manager.
	 *
	 * @param material_name name of a mesh existing in the material manager
	 */
	void set_mesh(const std::string &mesh_name) { mesh_name_property = mesh_name; }

	/**
	 * Set a custom instantiation of a mesh.
	 */
	void set_mesh(const MeshPtr &new_mesh) { mesh = new_mesh; }

	virtual const MeshPtr &get_mesh() const override { return mesh; }

	virtual bool has_custom_render() const override { return false; }

	virtual void custom_render(Camera & /*camera*/) override {}

	virtual void update_uniforms(Camera &camera, const std::string &render_pass_name) override;

	/**
	 * If the mesh has normals, update the normal matrix based on the camera's view matrix.
	 */
	void update_normal_matrix(const glm::mat4 &view_matrix);

public:
	static std::string get_static_type() { return GM_COMPONENT_RENDERABLE; }

private:
	void set_up_uniform_listeners();

	glm::mat3 make_normal_matrix(const glm::mat4 &view_matrix) const;

private:
	RenderSystemPtr render_system;

	MaterialManagerPtr material_manager;
	MeshManagerPtr mesh_manager;

	unsigned int render_layers;

	Property<glm::mat4> world_matrix_property;
	Property<glm::mat4> object_matrix_property;
	Property<glm::mat3> normal_matrix_property;

	Property<bool> culled_property;
	Property<bool> visible_property;

	Property<std::string> material_name_property;
	Property<std::string> mesh_name_property;

	Property<glm::vec3> mesh_pivot_point_offset_property;

	clan::SlotContainer slots;

	MaterialPtr material;
	MeshPtr mesh;

	std::map<std::string, PropertyToUniformConnector> uniforms_for_render_pass;
};

} // namespace Framework
} // namesoace GM
