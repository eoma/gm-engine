#include "GM/Application/MainComponentSerializer.h"

#include "GM/Application/Main.h"

#include "GM/Framework/EntityManager.h"

#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/Components/Transform.h"
#include "GM/Framework/Components/Light.h"
#include "GM/Framework/Components/Tessellate.h"
#include "GM/Framework/Components/StandardPass.h"

#include "GM/Framework/Utilities/Tools.h"

#include "GM/Framework/Entity.h"

#include <iostream>
#include <sstream>

namespace GM {
namespace Application {

MainComponentSerializer::MainComponentSerializer(Main *currentApp) : app(currentApp) {
	slots.connect(
		app->get_entity_manager()->register_component_serializer_signal(),
		this, &MainComponentSerializer::create_and_add_component);

	// Set up the default component creators
	add_component_creator(Framework::Camera::get_static_type(),
		[this](const Framework::EntityPtr &owner) {
			owner->create_component<Framework::Camera>(app->get_render_system(), app->get_texture_manager());
		});
	
	add_component_creator(Framework::Renderable::get_static_type(),
		[this](const Framework::EntityPtr &owner) {
			owner->create_component<Framework::Renderable>(app->get_render_system(), app->get_material_manager(), app->get_mesh_manager());
		});
	
	add_component_creator(Framework::Transform::get_static_type(),
		[this](const Framework::EntityPtr &owner) {
			owner->create_component<Framework::Transform>(app->get_scene_system());
		});
	
	add_component_creator(Framework::Light::get_static_type(),
		[this](const Framework::EntityPtr &owner) {
			owner->create_component<Framework::Light>(app->get_render_system());
		});
	
	add_component_creator(Framework::Tessellate::get_static_type(),
		[this](const Framework::EntityPtr &owner) {
			owner->create_component<Framework::Tessellate>(app->get_render_system());
		});
	
	add_component_creator(Framework::StandardPass::get_static_type(),
		[this](const Framework::EntityPtr &owner) {
			owner->create_component<Framework::StandardPass>();
		});
}

void MainComponentSerializer::create_and_add_component(const Framework::EntityPtr &owner, const std::string &type, const std::string &/*name*/) {
	auto iter = component_creators.find(type);

	if (iter != component_creators.end())
	{
		iter->second(owner);
	}
	else
	{
		std::clog << "Unable to find compoent creator for type " << type << std::endl;
	}
}

void MainComponentSerializer::add_component_creator(const std::string &component_type_name, std::function<void(const Framework::EntityPtr &)> &&creator_function)
{
	auto iter = component_creators.find(component_type_name);

	if (iter == component_creators.end())
	{
		iter = component_creators.emplace(component_type_name, creator_function).first;
	}
	else
	{
		// TODO: allow overrides?
		std::cerr << "MainComponentSerializer: " << component_type_name << "'s creator function will be overrided!";
		iter->second = creator_function;
	}
}

} // namespace Application
} // namespace GM
