#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Managers/MaterialManager.h"
#include "GM/Framework/Managers/MeshManager.h"
#include "GM/Framework/Systems/RenderSystem.h"
#include "GM/Core/GL/Render.h"

using namespace GM::Framework;

Renderable::Renderable(const EntityPtr &owner, const RenderSystemPtr &render_system, const MaterialManagerPtr material_manager, const MeshManagerPtr mesh_manager, const unsigned int render_layers, const std::string &name)
: Component<Renderable>(owner, name)
, render_system(render_system)
, material_manager(material_manager)
, mesh_manager(mesh_manager)
, render_layers(render_layers)
{
	render_system->add_renderable(this);

	world_matrix_property = owner->add(PROPERTY_WORLD_MATRIX, glm::mat4());
	object_matrix_property = owner->add(PROPERTY_OBJECT_MATRIX, glm::mat4());

	culled_property = owner->add(PROPERTY_CULLED, false);
	visible_property = owner->add(PROPERTY_VISIBLE, false);

	material_name_property = owner->add(PROPERTY_MATERIAL_NAME, std::string());
	slots.connect(material_name_property.value_changed(),
		[this](const std::string &/*old_material_name*/, const std::string &new_material_name) mutable
		{
			if (new_material_name.empty())
			{
				material = nullptr;
			}
			else
			{
				material = this->material_manager->get_or_create(new_material_name);
			}
		});

	mesh_name_property = owner->add(PROPERTY_MESH_NAME, std::string());
	slots.connect(mesh_name_property.value_changed(),
		[this](const std::string &/*old_mesh_name*/, const std::string &new_mesh_name) mutable
		{
			if (new_mesh_name.empty())
			{
				mesh = nullptr;
			}
			else
			{
				mesh = this->mesh_manager->get(new_mesh_name);
			}
		});
}

Renderable::~Renderable() {
	render_system->remove_renderable(this);
}
