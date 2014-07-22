#include "GM/Application/Main.h"

#include "GM/Framework/Systems/SceneSystem.h"
#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Framework/EntityManager.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Framework/IO/SoilTextureIO.h"

#include <glm/ext.hpp>

#include <iostream>

namespace GM {
namespace Application {

	Main::Main(const std::string &title, Main::Flags flags, Main::ErrorFlags error_flags, unsigned int width, unsigned int height, bool fullscreen)
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
, keep_running(add<bool>("keep_running", false))

#ifdef __APPLE__
, gl_version(3,2)
#else
, gl_version(4,3)
#endif

, game_time(30, 0)

, slots()
, initialize_sign()
, update_sign()
, prepare_sign()
, render_sign()
, clean_up_sign()
{
	slots.connect(this->title.value_changed(), this, &Main::on_title_changed);
	slots.connect(this->resolution.value_changed(), this, &Main::on_resolution_changed);
	slots.connect(this->fullscreen.value_changed(), this, &Main::on_fullscreen_changed);

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

Main::~Main()
{
}

void Main::run() {

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

	init_window_and_gl();
	
	keep_running = true;

	initialize();

	while(is_running())
	{
		update();
		prepare();
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwWindowShouldClose(window))
		{
			stop_running();
		}
	}

	clean_up();

	glfwDestroyWindow(window);
	glfwTerminate();
	// TODO: destroy context, window
}

void Main::initialize() {
	initialize_sign();
}
void Main::update() {

	game_time.update();
	auto elapsed_time = game_time.get_time_elapsed();

	if (has_entity_manager()) {
		entity_manager->update(elapsed_time);
	}

	update_sign(elapsed_time);
}
void Main::prepare() {

	if (has_scene_system()) {
		scene_system->prepare();
	}

	prepare_sign();
}
void Main::render() {

	if (has_render_system()) {
		render_system->render();
	}

	render_sign();
}
void Main::clean_up() {
	clean_up_sign();
}

void Main::on_title_changed(const std::string &/*old_value*/, const std::string &new_value) {
	if (is_running())
	{
		glfwSetWindowTitle(window, new_value.c_str());
	}
}

void Main::on_resolution_changed(const glm::uvec2 &/*old_value*/, const glm::uvec2 &new_value) {
	if (is_running())
	{
		glfwSetWindowSize(window, new_value.x, new_value.y);
	}
}

void Main::on_fullscreen_changed(const bool &/*old_value*/, const bool &new_value) {
	// TODO: Update fullscreen state for GLFW
	if (is_running())
	{
		
	}
}

void Main::init_window_and_gl()
{
	glfwSetErrorCallback([](int error_code, const char * error_string){
		std::cerr << "GLFW received error code " 
			<< std::hex << std::showbase << error_code
			<< " and error message: " << error_string << std::endl;
	});

	if (!glfwInit())
	{
		glfwTerminate();
		// FIXME: Should probably just say stop_running() instead?
		exit(EXIT_FAILURE);
	}

	GLFWmonitor *fullscreen_monitor = nullptr;
	if (fullscreen)
	{
		fullscreen_monitor = glfwGetPrimaryMonitor();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version.x);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version.y);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(
		resolution.get().x, resolution.get().y,
		title.get().c_str(),
		fullscreen_monitor,
		nullptr
	);
	if (!window)
	{
		glfwTerminate();
		// FIXME: Should probably just say stop_running() instead?
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	gl3wInit();

	std::cerr << "VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cerr << "VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::cerr << "RENDERER: " << glGetString(GL_RENDERER) << std::endl;

	glm::ivec2 context_gl_version;
	glGetIntegerv(GL_MAJOR_VERSION, &context_gl_version.x);
	glGetIntegerv(GL_MINOR_VERSION, &context_gl_version.y);

	if (gl_version != context_gl_version) {
		std::cerr << "Requested GL version: " << glm::to_string(gl_version) << std::endl;
		std::cerr << "Got GL version: " << glm::to_string(context_gl_version) << std::endl;
	}
}

void Main::set_gl_version(int major, int minor)
{
	if (!is_running())
	{
#ifndef __APPLE__ // On Mac OS X, you will get the version supported by the os, OS X 10.9 supports 4.1
		gl_version.x = major;
		gl_version.y = minor;
#endif
	}
	else
	{
		throw std::runtime_error("You should not set GL version while run() is running");
	}
}

} // namespace Application
} // namespace GM
