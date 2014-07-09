#include <cstdlib>
#include <GM/Core/Utilities/VaoLayout.h>

#include <memory>

using namespace GM;
using namespace Core;

// FIXME: Remove when GL is introduced
#ifndef GL_ARRAY_BUFFER
#	define GL_ARRAY_BUFFER 1
#endif

#ifndef GL_ELEMENT_ARRAY_BUFFER
#	define GL_ELEMENT_ARRAY_BUFFER 2
#endif

#define POSITION 0
#define NORMAL 1
#define INDEX 2

bool mainTest() {

	{
		VaoLayout layout;

		layout
			.for_buffer(1)
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
			layout.for_buffer(0);
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
			layout.for_buffer(1)
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
