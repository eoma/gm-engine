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
