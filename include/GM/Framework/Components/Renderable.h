#pragma once

#include "IRenderable.h"
#include "../DefinitionsComponentNames.h"

#include "../Totem/Component.h"

#include "GM/Core/Utilities/UniformOperations.h"

#include <memory>
#include <string>

namespace GM {
namespace Framework {

class MeshManager; typedef std::shared_ptr<MeshManager> MeshManagerPtr;
class MaterialManager; typedef std::shared_ptr<MaterialManager> MaterialManagerPtr;

class Renderable; typedef std::shared_ptr<Renderable> RenderablePtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Renderable : public IRenderable, public Component<Renderable> {
public:
	Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const MaterialManagerPtr material_manager, const MeshManagerPtr mesh_ptr, unsigned int render_layers = 1, const std::string &name = std::string());
	virtual ~Renderable();

	std::string get_type() const override { return get_static_type(); }

	virtual	glm::mat4 get_world_matrix() const override { return world_matrix_property; }
	virtual	glm::mat4 get_object_matrix() const override { return object_matrix_property; }

	virtual bool is_culled() const override { return culled_property; }
	virtual bool is_invisible() const override { return !visible_property || is_culled(); }

	virtual unsigned int get_render_layers() const override { return render_layers; }

	void set_material(const std::string& material_name) { material_name_property = material_name; }
	virtual const MaterialPtr &get_material() const override { return material; }

	void set_mesh(const std::string &mesh_name) { mesh_name_property = mesh_name; }
	virtual const MeshPtr &get_mesh() const override { return mesh; }

	virtual bool has_custom_render() const override { return false; }

	virtual void custom_render(Camera * /*camera*/) override {}

	virtual void update_uniforms(Camera *camera) override; //{ update_uniforms_signal(camera); }

public:
	static std::string get_static_type() { return COMPONENT_RENDERABLE; }

private:
	void set_up_uniforms();

	void add_uniform_listener(const std::shared_ptr<IProperty> &prop);

	template<class Value>
	void add_uniform(const std::shared_ptr<IProperty> &prop, const unsigned int program, const int uniform_location);

private:
	RenderSystemPtr render_system;

	MaterialManagerPtr material_manager;
	MeshManagerPtr mesh_manager;

	const unsigned int render_layers;

	Property<glm::mat4> world_matrix_property;
	Property<glm::mat4> object_matrix_property;

	Property<bool> culled_property;
	Property<bool> visible_property;

	Property<std::string> material_name_property;
	Property<std::string> mesh_name_property;

	clan::SlotContainer slots;

	MaterialPtr material;
	MeshPtr mesh;

	clan::Signal<void()> update_uniforms_signal;
	clan::Slot new_uniform_listener_slot;
	clan::SlotContainer update_uniform_slots;
};

//
// Implementations
//

template<class Value>
void Renderable::add_uniform(const std::shared_ptr<IProperty> &prop, const unsigned int program, const int uniform_location)
{
	if (IProperty::is_type<Value>(prop)) {
		auto prop_with_type = std::static_pointer_cast<Property<Value>>(prop);
		update_uniform_slots.connect(update_uniforms_signal, [program, uniform_location, prop_with_type]() {
			Core::update_uniform(program, uniform_location, prop_with_type->get());
		});
	}
	else
	{
		throw clan::Exception(clan::string_format("Property (%1) is to be added as uniform, but is of wrong type!", prop->get_name()));
	}
}

} // namespace Framework
} // namesoace GM
