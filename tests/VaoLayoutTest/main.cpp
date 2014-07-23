#include <cstdlib>
#include <GM/Core/GL/BufferObject.h>
#include <GM/Core/Utilities/VaoLayout.h>

#include <GM/Application/Main.h>

#include <memory>

using namespace GM;
using namespace Core;
using namespace Application;

#define POSITION 0
#define NORMAL 1
#define INDEX 2

bool mainTest() {
	auto app = Main::create_with_gl_version("VaoLayoutTest", 3, 3);

	auto buffer = std::make_shared<BufferObject>(1000);

	{
		VaoLayout layout;

		layout
			.for_buffer(buffer)
				.use_as(GL_ARRAY_BUFFER)
					.bind(POSITION, 3, 1, false, 7)
					.bind(NORMAL, 4, 1, false, 7)
				.use_as(GL_ELEMENT_ARRAY_BUFFER)
		;
	}

	{
		VaoLayout layout;
		
		bool exception_thrown = false;

		try {
			layout.use_as(GL_ARRAY_BUFFER);
		} catch (std::runtime_error e) {
			exception_thrown = true;
		}

		if (!exception_thrown) {
			throw std::runtime_error("Expected exception to be thrown");
		}
	}

	{
		VaoLayout layout;
		
		bool exception_thrown = false;

		try {
			layout.for_buffer(nullptr);
		} catch (std::runtime_error e) {
			exception_thrown = true;
		}

		if (!exception_thrown) {
			throw std::runtime_error("Expected exception to be thrown");
		}
	}

	{
		VaoLayout layout;
		
		bool exception_thrown = false;

		try {
			layout.for_buffer(buffer)
				.use_as(GL_ELEMENT_ARRAY_BUFFER)
					.bind(1, 2, 3, false, 0, 0 ,0);
		} catch (std::runtime_error e) {
			exception_thrown = true;
		}

		if (!exception_thrown) {
			throw std::runtime_error("Expected exception to be thrown");
		}
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
