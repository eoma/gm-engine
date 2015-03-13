#include "GM/Application/Main.h"

#include "GM/Application/MainComponentSerializer.h"

#include "GM/Framework/Systems/SceneSystem.h"
#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Framework/EntityManager.h"
#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/MaterialManager.h"
#include "GM/Framework/Managers/MeshManager.h"
#include "GM/Framework/Managers/ShaderManager.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Managers/VaoManager.h"
#include "GM/Framework/Managers/UniformBufferBlockManager.h"

#include "GM/Framework/IO/SoilImageIO.h"
#include "GM/Framework/IO/AssimpMeshIO.h"

#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/Renderable.h"
#include "GM/Framework/Components/Transform.h"
#include "GM/Framework/Components/Light.h"
#include "GM/Framework/Components/Tessellate.h"
#include "GM/Framework/Components/StandardPass.h"

#include "GM/Framework/Primitives/TrianglePrimitive.h"
#include "GM/Framework/Primitives/QuadXYPrimitive.h"
#include "GM/Framework/Primitives/QuadXZPrimitive.h"
#include "GM/Framework/Primitives/FullscreenQuadPrimitive.h"
#include "GM/Framework/Primitives/SkyboxPrimitive.h"
#include "GM/Framework/Primitives/CubePrimitive.h"
#include "GM/Framework/Primitives/IcosahedronPrimitive.h"

#include "GM/Framework/Utilities/Tools.h"
#include "GM/Framework/Utilities/CameraMatricesUbo.h"
#include "GM/Framework/Utilities/LightListUbo.h"

#include "GM/Framework/Entity.h"

#include <iostream>
#include <sstream>

namespace GM {
namespace Application {

Main::Main(const std::string &title, Main::Flags flags, Main::ErrorFlags error_flags, unsigned int width, unsigned int height, bool fullscreen, bool visible, bool construct_window)
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
, uniform_buffer_block_manager()

, title(add<std::string>("title", title))
, resolution(add<glm::uvec2>("width", glm::uvec2(width, height)))
, fullscreen(add<bool>("fullscreen", fullscreen))
, visible(add<bool>("visible", visible))
, keep_running(add<bool>("keep_running", false))
, initialize_entities(true)

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
	slots.connect(this->visible.value_changed(), this, &Main::on_visible_changed);

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
			std::make_shared<Framework::SoilImageIO>()
			);
	}

	if (flags & GM_FRAMEWORK_MATERIAL_MANAGER)
	{
		material_manager = std::make_shared<Framework::MaterialManager>(shader_manager, texture_manager);
	}

	if (flags & GM_FRAMEWORK_VAO_MANAGER)
	{
		vao_manager = std::make_shared<Framework::VaoManager>();
	}

	if (flags & GM_FRAMEWORK_UNIFORM_BUFFER_BLOCK_MANAGER)
	{
		uniform_buffer_block_manager = std::make_shared<Framework::UniformBufferBlockManager>(false);

		if (shader_manager) {
			// Set up an added-shader-listener and scan and bind shaders for uniform buffer blocks.
			slots.connect(shader_manager->on_shader_added(),
				[this](const std::string &/*name*/, const Core::ShaderPtr &shader) {
					uniform_buffer_block_manager->bind_shader(shader);
				}
			);
		}
	}

	if (flags & GM_FRAMEWORK_MESH_MANAGER)
	{
		mesh_manager = std::make_shared<Framework::MeshManager>(buffer_manager, vao_manager,
			std::make_shared<Framework::AssimpMeshIO>()
			);

		if (flags & GM_FRAMEWORK_PRIMITIVES)
		{
			mesh_manager->add_primitive(std::make_shared<Framework::TrianglePrimitive>());
			mesh_manager->add_primitive(std::make_shared<Framework::QuadXYPrimitive>());
			mesh_manager->add_primitive(std::make_shared<Framework::QuadXZPrimitive>());
			mesh_manager->add_primitive(std::make_shared<Framework::FullscreenQuadPrimitive>());
			mesh_manager->add_primitive(std::make_shared<Framework::SkyboxPrimitive>());
			mesh_manager->add_primitive(std::make_shared<Framework::CubePrimitive>());
			mesh_manager->add_primitive(std::make_shared<Framework::IcosahedronPrimitive>());
		}
	}

	if (flags & GM_FRAMEWORK_COMPONENT_SERIALIZER)
	{
		main_component_serializer = std::make_shared<MainComponentSerializer>(this);
	}

	if (construct_window)
	{
		construct_window_and_gl();
	}

	keyboard_state.resize(GLFW_KEY_LAST, false);
	button_state.resize(GLFW_MOUSE_BUTTON_LAST, false);
	mouse_position = glm::vec2();
}

Main::~Main()
{
	destruct_window_and_gl();
}

MainPtr Main::create_with_no_context(const std::string &title, Main::Flags flags, Main::ErrorFlags error_flags)
{
	return std::make_shared<Main>(title, flags, error_flags, 800, 640, false, true, false);
}

MainPtr Main::create_with_gl_version(const std::string &title, unsigned int major, unsigned int minor, bool visible, bool construct_context, Main::Flags flags, Main::ErrorFlags error_flags)
{
	auto app = std::make_shared<Main>(title, flags, error_flags, 800, 640, false, visible, false);
	app->set_gl_version(major, minor);

	if (construct_context)
	{
		app->construct_window_and_gl();
	}

	return app;
}

void Main::run(bool destruct_window_and_gl_on_exit)
{

	//Test if we should check for null systems
	if (error_flags & GM_ERROR_NULL_SYSTEM)
	{
		if (!has_scene_system()) {
			throw clan::Exception("Must have scene system in order to run!");
		}

		if (!has_render_system()) {
			throw clan::Exception("Must have render system in order to run!");
		}

		if (!has_entity_manager()) {
			throw clan::Exception("Must have entity manager in order to run!");
		}

		if (!has_buffer_manager()) {
			throw clan::Exception("Must have buffer manager in order to run!");
		}

		if (!has_material_manager()) {
			throw clan::Exception("Must have material manager in order to run!");
		}

		if (!has_mesh_manager()) {
			throw clan::Exception("Must have mesh manager in order to run!");
		}

		if (!has_shader_manager()) {
			throw clan::Exception("Must have shader manager in order to run!");
		}

		if (!has_texture_manager()) {
			throw clan::Exception("Must have texture manager in order to run!");
		}

		if (!has_vao_manager()) {
			throw clan::Exception("Must have vao manager in order to run!");
		}
	}

	construct_window_and_gl();
	
	keep_running = true;

	initialize();

	while(is_running())
	{
		update();
		prepare();
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
		//glfwWaitEvents();

		if (glfwWindowShouldClose(window))
		{
			stop_running();
		}
	}

	clean_up();

	if (destruct_window_and_gl_on_exit)
	{
		destruct_window_and_gl();
	}
}

void Main::initialize()
{
	if (has_render_system())
	{
		render_system->resize(resolution.get().x, resolution.get().y);
	}

	if (initialize_entities) {
		entity_manager->initialize();
	}

	reset_mouse_position();
	initialize_sign();
}

void Main::update()
{

	game_time.update();
	auto elapsed_time = game_time.get_time_elapsed();

	if (has_entity_manager())
	{
		entity_manager->update(elapsed_time);
	}

	update_sign(elapsed_time);
}

void Main::prepare()
{

	if (has_scene_system())
	{
		scene_system->prepare();
	}

	prepare_sign();
}

void Main::render()
{

	if (has_render_system())
	{
		render_system->render();
	}

	render_sign();
}

void Main::clean_up()
{
	clean_up_sign();
}

void Main::on_title_changed(const std::string &/*old_value*/, const std::string &new_value)
{
	if (is_context_setup())
	{
		glfwSetWindowTitle(window, new_value.c_str());
	}
}

void Main::on_resolution_changed(const glm::uvec2 &/*old_value*/, const glm::uvec2 &new_value)
{
	if (is_context_setup())
	{
		glfwSetWindowSize(window, new_value.x, new_value.y);
	}
}

void Main::on_fullscreen_changed(const bool &/*old_value*/, const bool &new_value)
{
	// TODO: Update fullscreen state for GLFW
	if (is_context_setup())
	{
		
	}
}

void Main::on_visible_changed(const bool &/*old*value*/, const bool &visible)
{
	if (is_context_setup())
	{
		if (visible)
		{
			glfwShowWindow(window);
		}
		else
		{
			glfwHideWindow(window);
		}
	}
}

void Main::construct_window_and_gl()
{
	if (is_running() || is_context_setup())
	{
		// Should not be possible to construct multiple windows and contexts in same object
		return;
	}

	glfwSetErrorCallback(&Main::glfw_error_callback);

	if (!glfwInit())
	{
		// Failed to initialize 
		glfwTerminate();
		throw clan::Exception("Unable to initialize glfw!");
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
	glfwWindowHint(GLFW_VISIBLE, visible);

	if (error_flags & ErrorFlags::GM_ERROR_GL_DEBUG_OUTPUT)
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}

	window = glfwCreateWindow(
		resolution.get().x, resolution.get().y,
		title.get().c_str(),
		fullscreen_monitor,
		nullptr
	);
	if (!window)
	{
		glfwTerminate();
		throw clan::Exception("Unable to create window or OpenGL context!");
	}

	glfwSetWindowUserPointer(window, this);

	// Set callbacks for our window
	glfwSetMouseButtonCallback(window, &Main::mouse_button_callback);
	glfwSetCursorPosCallback(window, &Main::cursor_position_callback);
	glfwSetCursorEnterCallback(window, &Main::cursor_enter_callback);
	glfwSetScrollCallback(window, &Main::scroll_callback);
	glfwSetKeyCallback(window, &Main::keyboard_callback);
	glfwSetCharCallback(window, &Main::keyboard_unicode_callback);
	glfwSetFramebufferSizeCallback(window, &Main::window_size_callback);

	glfwMakeContextCurrent(window);

	gl3wInit();

	if (error_flags & ErrorFlags::GM_ERROR_GL_DEBUG_OUTPUT)
	{
		if ( glDebugMessageCallback ) // Debug message callback may not exist if GL < 4.3
		{
			glDebugMessageCallback(gl_debug_callback, NULL);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
	}

	std::clog << "VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::clog << "VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::clog << "RENDERER: " << glGetString(GL_RENDERER) << std::endl;

	glm::ivec2 context_gl_version;
	glGetIntegerv(GL_MAJOR_VERSION, &context_gl_version.x);
	glGetIntegerv(GL_MINOR_VERSION, &context_gl_version.y);

	if (gl_version != context_gl_version) {
		std::clog << "Requested GL version: " << gl_version.x << "." << gl_version.y << std::endl;
		std::clog << "Got GL version: " << context_gl_version.x << "." << context_gl_version.y << std::endl;
	}

	if (has_uniform_buffer_block_manager())
	{
		uniform_buffer_block_manager->initialize();
	}

	if (has_render_system())
	{
		render_system->set_camera_matrices_ubo(std::make_shared<Framework::CameraMatricesUbo>(buffer_manager, uniform_buffer_block_manager));
		render_system->set_light_list_ubo(std::make_shared<Framework::LightListUbo>(buffer_manager, uniform_buffer_block_manager));
	}
}

void Main::destruct_window_and_gl()
{
	if (is_running() || !is_context_setup())
	{
		// throw exception?
		return;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;
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
		throw clan::Exception("You should not set GL version while run() is running");
	}
}


void Main::gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
    std::string error = gl_format_debug_output(source, type, id, severity, message);
    std::cout << error << std::endl;

#ifdef _MSC_VER
	if (type == GL_DEBUG_TYPE_ERROR_ARB || type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB || severity == GL_DEBUG_SEVERITY_HIGH_ARB)
		__debugbreak();
#endif
}

std::string Main::gl_format_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    std::stringstream stringStream;
    std::string sourceString;
    std::string typeString;
    std::string severityString;
 
    // The AMD variant of this extension provides a less detailed classification of the error,
    // which is why some arguments might be "Unknown".
    switch (source) {
        //case GL_DEBUG_CATEGORY_API_ERROR_AMD:
        case GL_DEBUG_SOURCE_API: {
            sourceString = "API";
            break;
        }
        //case GL_DEBUG_CATEGORY_APPLICATION_AMD:
        case GL_DEBUG_SOURCE_APPLICATION: {
            sourceString = "Application";
            break;
        }
        //case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
            sourceString = "Window System";
            break;
        }
        //case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
        case GL_DEBUG_SOURCE_SHADER_COMPILER: {
            sourceString = "Shader Compiler";
            break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY: {
            sourceString = "Third Party";
            break;
        }
        //case GL_DEBUG_CATEGORY_OTHER_AMD:
        case GL_DEBUG_SOURCE_OTHER: {
            sourceString = "Other";
            break;
        }
        default: {
            sourceString = "Unknown";
            break;
        }
    }
 
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: {
            typeString = "Error";
            break;
        }
        //case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
            typeString = "Deprecated Behavior";
            break;
        }
        //case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
            typeString = "Undefined Behavior";
            break;
        }
        case GL_DEBUG_TYPE_PORTABILITY_ARB: {
            typeString = "Portability";
            break;
        }
        //case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
        case GL_DEBUG_TYPE_PERFORMANCE: {
            typeString = "Performance";
            break;
        }
        //case GL_DEBUG_CATEGORY_OTHER_AMD:
        case GL_DEBUG_TYPE_OTHER: {
            typeString = "Other";
            break;
        }
        default: {
            typeString = "Unknown";
            break;
        }
    }
 
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: {
            severityString = "High";
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM: {
            severityString = "Medium";
            break;
        }
        case GL_DEBUG_SEVERITY_LOW: {
            severityString = "Low";
            break;
        }
        default: {
            severityString = "Unknown";
            break;
        }
    }
 
    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";
 
    return stringStream.str();
}

void Main::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));
	current_main->mouse_button_sign(button, action, mods);
}

void Main::cursor_position_callback(GLFWwindow *window, double screen_x, double screen_y)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));
	auto resolution = current_main->get_resolution();
	auto ratio = resolution.x / (float)resolution.y;
	auto x = ratio*(2 * screen_x / (float)resolution.x - 1);
	auto y = 2 * -screen_y / (float)resolution.y + 1;
	current_main->mouse_position = glm::vec2((float)x, (float)y);
	current_main->cursor_position_changed_sign(x, y);
}

void Main::cursor_enter_callback(GLFWwindow *window, int entered)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));
	current_main->cursor_entered_window_sign(entered == GL_TRUE ? true : false);
}

void Main::scroll_callback(GLFWwindow *window, double scroll_offset_x, double scroll_offset_y)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));
	current_main->scroll_wheel_changed_sign(scroll_offset_x, scroll_offset_y);
}

void Main::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));

	// Update keyboard state bookkeeping
	switch (action) {
	case GLFW_PRESS:
		current_main->keyboard_state[key] = true;
		break;
	case GLFW_RELEASE:
		current_main->keyboard_state[key] = false;
		break;
	};

	// Send out the signal
	current_main->keyboard_sign(key, scancode, action, mods);
}

void Main::keyboard_unicode_callback(GLFWwindow *window, unsigned int code_point)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));
	current_main->keyboard_unicode_sign(code_point);
}

void Main::window_size_callback(GLFWwindow* window, int width, int height)
{
	Main *current_main = static_cast<Main*>(glfwGetWindowUserPointer(window));
	current_main->set_resolution(width, height);
	if (current_main->has_render_system()) current_main->render_system->resize(width, height);
	current_main->window_size_sign(width, height);
}

bool Main::is_key_down(unsigned int key) const {
	if (key >= GLFW_KEY_LAST) {
		throw clan::Exception("Key was out of bounds!");
	}

	return keyboard_state[key];
}

bool Main::is_button_down(unsigned int button) const {
	if (button >= GLFW_MOUSE_BUTTON_LAST) {
		throw clan::Exception("Button was out of bounds!");
	}

	return button_state[button];
}

const glm::vec2 &Main::get_mouse_position() const {
	return mouse_position;
}

void Main::reset_mouse_position() {
	glfwSetCursorPos(window, (resolution.get().x / 2.0) + 1, resolution.get().y / 2.0);
}

void Main::show_cursor() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Main::hide_cursor() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Main::glfw_error_callback(int error_code, const char *error_msg) {
	std::cerr << "GLFW Error (0x" << std::hex << error_code << std::dec << "): " << error_msg << std::endl;
}

void Main::add_extra_resource_paths(const std::string &resource_file)
{
	std::string resource_path;

	try
	{
		resource_path = Framework::find_path_in_hierarchy(clan::System::get_exe_path(), resource_file);
	}
	catch (clan::Exception &e)
	{
		std::cerr << e.what() << std::endl; // is this necessary?
		return;
	}

	clan::JsonValue resources = clan::JsonValue::from_json(clan::File::read_text(resource_path));

	if (!resources.is_object())
	{
		return;
	}

	auto &map = resources.get_members();

	std::string root = "";

	if (map.find("root") != map.end())
	{
		root = map["root"].to_string();
	}

	if (has_texture_manager() && map.find("textures") != map.end())
	{
		texture_manager->add_texture_path(clan::PathHelp::make_absolute(root, map["textures"].to_string()));
	}

	if (has_mesh_manager() && map.find("mesh") != map.end())
	{
		mesh_manager->add_mesh_path(clan::PathHelp::make_absolute(root, map["mesh"].to_string()));
	}

	// Add more...
}

} // namespace Application
} // namespace GM
