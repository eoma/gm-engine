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
				s("void main() {\n") +
				s("    gl_Position = vec4(0.0, 0.0, 0.0, 0.0);\n") +
				s("}\n")
				,
				GL_VERTEX_SHADER),

			Core::ShaderSource("fragment",
				s("#version 330\n") +
				s("out vec4 out_FragColor;\n") +
				s("void main() {\n") +
				s("    out_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n") +
				s("}\n")
				,
				GL_FRAGMENT_SHADER),
		});
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
