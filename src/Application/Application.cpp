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

Application::Application(const std::string &title, Application::Flags flags, unsigned int width, unsigned int height, bool fullscreen)
: Framework::PropertyContainer<>()
, window(nullptr)

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

	if (flags & GM_SCENE_SYSTEM)
	{
		scene_system = std::make_shared<Framework::SceneSystem>();
	}

	if (flags & GM_RENDER_SYSTEM)
	{
		render_system = std::make_shared<Framework::RenderSystem>();
	}

	if (flags & GM_ENTITY_MANAGER)
	{
		entity_manager = std::make_shared<Framework::EntityManager>();
	}
	
	if (flags & GM_BUFFER_MANAGER)
	{
		buffer_manager = std::make_shared<Framework::BufferManager>();
	}

	if (flags & GM_SHADER_MANAGER)
	{
		shader_manager = std::make_shared<Framework::ShaderManager>();
	}
	
	if (flags & GM_TEXTURE_MANAGER)
	{
		texture_manager = std::make_shared<Framework::TextureManager>(
			std::make_shared<Framework::SoilTextureIO>()
		);
	}

	if (flags & GM_VAO_MANAGER)
	{
		vao_manager = std::make_shared<Framework::VaoManager>();
	}
}

void Application::run() {
	if (!has_render_system()) {
		throw std::runtime_error("Must have render system in order to run!");
	}

	// set up window, context...
	// TODO: check that context is valid

	initialize_sign();

	while(keep_running)
	{
		update_sign(0.1); // fix thiw with actual time
		prepare_sign();
		render_sign();
	}

	clean_up_sign();

	// destroy context, window
}

} // namespace Application
} // namespace GM
