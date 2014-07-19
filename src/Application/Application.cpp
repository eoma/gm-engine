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
, resolution(add<glm::uvec2>("width", glm::uvec2(width, height)))
, fullscreen(add<bool>("fullscreen", fullscreen))
, keep_running(add<bool>("keep_running", true))

, game_time(30, 0)

, slots()
, initialize_sign()
, update_sign()
, prepare_sign()
, render_sign()
, clean_up_sign()
{
	slots.connect(this->title.value_changed(), this, &Application::on_title_changed);
	slots.connect(this->resolution.value_changed(), this, &Application::on_resolution_changed);
	slots.connect(this->fullscreen.value_changed(), this, &Application::on_fullscreen_changed);

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

	game_time.update();
	auto elapsed_time = game_time.get_time_elapsed();

	if (has_entity_manager()) {
		entity_manager->update(elapsed_time);
	}

	update_sign(elapsed_time);
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

void Application::on_title_changed(const std::string &old_value, const std::string &new_value) {
	// TODO: Update title caption in GLFW
}

void Application::on_resolution_changed(const glm::uvec2 &old_value, const glm::uvec2 &new_value) {
	// TODO: Update window resolution for GLFW
}

void Application::on_fullscreen_changed(const bool &old_value, const bool &new_value) {
	// TODO: Update fullscreen state for GLFW
}

} // namespace Application
} // namespace GM
