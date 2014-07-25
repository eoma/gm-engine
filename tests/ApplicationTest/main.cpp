#include "GM/Application/Main.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

using namespace GM;
using namespace Application;

bool mainTest() {
	auto app = Main::create_with_gl_version("test", 3, 3);

	bool update_called = false;
	auto update_slot = app->on_update().connect([&](float /*value*/) mutable {
		app->stop_running();
		update_called = true;
	});

	app->run();

	if (!update_called)
	{
		throw clan::Exception("Applications' update signal were not invoked");
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
