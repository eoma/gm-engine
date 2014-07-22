#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Application/Main.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

using namespace GM;
using namespace Application;

bool mainTest() {
	Main app("test");

	app.set_gl_version(3, 3);

	auto keep_running = app.add<bool>("keep_running", false);
	bool update_called = false;

	auto initialize_slot = app.on_initialize().connect([]() {
		Core::ShaderPtr shader = nullptr;

		typedef std::string s;

		shader = Core::ShaderFactory::make_program({
			Core::ShaderSource("vertex",
				s("#version 330\n") +
				s("in vec4 position_ws;\n") +
				s("uniform int non_active_uniform = 0;\n") +
				s("uniform int active_uniform = 1;\n") +
				s("struct MyType {\n") + 
				s("    ivec3 something_array[3];\n") +
				s("    float some_float;\n") +
				s("};\n") +
				s("uniform MyType my_type_collection[3];") +
				s("void main() {\n") +
				s("    gl_Position = position_ws + vec4(active_uniform, 0.0, 0.0, 0.0);\n") +
				s("    gl_Position.x = my_type_collection[1].some_float;\n") +
				s("    for (int i = 0; i < 3; ++i) gl_Position.xyz += my_type_collection[i].something_array[0];\n") +
				s("}\n")
				,
				GL_VERTEX_SHADER),

			Core::ShaderSource("fragment",
				s("#version 330\n") +
				s("out vec4 out_FragColor;\n") +
				s("uniform vec3 prim_color = vec3(1.0, 0.0, 0.0);\n") +
				s("uniform vec3 sec_colors[2];\n") +
				s("void main() {\n") +
				s("    out_FragColor = vec4(prim_color + sec_colors[0] + sec_colors[1], 1.0);\n") +
				s("}\n")
				,
				GL_FRAGMENT_SHADER),
		});

		for (const auto &info : shader->get_attribute_info())
		{
			std::cout << "Attribute: " << info.name << std::endl;
			std::cout << "Type: " << std::hex << std::showbase << info.type << std::dec << std::endl;
			std::cout << "Size: " << info.size << std::endl;
			std::cout << "Location: " << info.location << std::endl;

			std::cout << std::endl;
		}

		std::cout << "-----" << std::endl << std::endl;

		for (const auto &info : shader->get_uniform_info())
		{
			std::cout << "Uniform: " << info.name << std::endl;
			std::cout << "Type: " << std::hex << std::showbase << info.type << std::dec << std::endl;
			std::cout << "Size: " << info.size << std::endl;
			std::cout << "Location: " << info.location << std::endl;

			std::cout << std::endl;
		}
	});

	auto update_slot = app.on_update().connect([&](float value) mutable {
		keep_running = false;
		update_called = true;
	});

	app.run();

	if (!update_called)
	{
		throw std::runtime_error("Applications' update signal were not invoked");
	}

	return true;
}

int main() {
	
	bool successful = mainTest();

#ifdef WIN32
	system("pause");
#endif

	if (successful) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}