#include "GM/Application/Application.h"

#include "GM/Framework/Systems/SceneSystem.h"
#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Framework/EntityManager.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Framework/IO/SoilTextureIO.h"

namespace GM {
namespace Application {

	Application::Application(const std::string &title, Application::Flags flags, Application::ErrorFlags error_flags, unsigned int width, unsigned int height, bool fullscreen)
: Framework::PropertyContainer<>()
, window(nullptr)
, error_flags(error_flags)

, scene_system()
, render_system()
, entity_manager()
, buffer_manager()
, shader_manager()
, texture_manager()
, vao_manager()

, title(add<std::string>("title", title))
, width(add<unsigned int>("width", width))
, height(add<unsigned int>("height", height))
, fullscreen(add<bool>("fullscreen", fullscreen))
, keep_running(add<bool>("keep_running", true))

, slots()
, initialize_sign()
, update_sign()
, prepare_sign()
, render_sign()
, clean_up_sign()
{

	if (flags & GM_FRAMEWORK_SCENE_SYSTEM)
	{
		scene_system = std::make_shared<Framework::SceneSystem>();
	}

	if (flags & GM_FRAMEWORK_RENDER_SYSTEM)
	{
		render_system = std::make_shared<Framework::RenderSystem>();
	}

	if (flags & GM_FRAMEWORK_ENTITY_MANAGER)
	{
		entity_manager = std::make_shared<Framework::EntityManager>();
	}
	
	if (flags & GM_FRAMEWORK_BUFFER_MANAGER)
	{
		buffer_manager = std::make_shared<Framework::BufferManager>();
	}

	if (flags & GM_FRAMEWORK_SHADER_MANAGER)
	{
		shader_manager = std::make_shared<Framework::ShaderManager>();
	}
	
	if (flags & GM_FRAMEWORK_TEXTURE_MANAGER)
	{
		texture_manager = std::make_shared<Framework::TextureManager>(
			std::make_shared<Framework::SoilTextureIO>()
		);
	}

	if (flags & GM_FRAMEWORK_VAO_MANAGER)
	{
		vao_manager = std::make_shared<Framework::VaoManager>();
	}
}

void Application::run() {

	//Test if we should check for null systems
	if (error_flags & GM_ERROR_NULL_SYSTEM)
	{
		if (!has_scene_system()) {
			throw std::runtime_error("Must have scene system in order to run!");
		}

		if (!has_render_system()) {
			throw std::runtime_error("Must have render system in order to run!");
		}

		if (!has_entity_manager()) {
			throw std::runtime_error("Must have entity manager in order to run!");
		}

		if (!has_buffer_manager()) {
			throw std::runtime_error("Must have buffer manager in order to run!");
		}

		if (!has_shader_manager()) {
			throw std::runtime_error("Must have shader manager in order to run!");
		}

		if (!has_texture_manager()) {
			throw std::runtime_error("Must have texture manager in order to run!");
		}

		if (!has_vao_manager()) {
			throw std::runtime_error("Must have vao manager in order to run!");
		}
	}

	// TODO: set up window, context...
	// TODO: check that context is valid

	initialize();

	while(is_running())
	{
		update();
		prepare();
		render();
	}

	clean_up();

	// TODO: destroy context, window
}

void Application::initialize() {
	initialize_sign();
}
void Application::update() {

	if (has_entity_manager()) {
		entity_manager->update();
	}

	update_sign(0.1); // FIXME: fix this with actual time
}
void Application::prepare() {

	if (has_scene_system()) {
		scene_system->prepare();
	}

	prepare_sign();
}
void Application::render() {

	if (has_render_system()) {
		render_system->render();
	}

	render_sign();
}
void Application::clean_up() {
	clean_up_sign();
}

} // namespace Application
} // namespace GM
