#include <cstdlib>
#include <GM/Core/Utilities/VaoLayout.h>
#include <GM/Framework/Managers/BufferManager.h>
#include <GM/Framework/Managers/VaoManager.h>

#include <iostream>
#include <memory>

using namespace GM;
using namespace Core;
using namespace Framework;

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

	auto buffer_manager = std::make_shared<BufferManager>();
	auto vao_manager = std::make_shared<VaoManager>();

	// Buffer should be allocated before we start our VAO definition
	auto allocated1 = buffer_manager->allocate(3000);
	auto allocated2 = buffer_manager->allocate(1000);
	auto allocated3 = buffer_manager->allocate(2000, true);

	// Do stuff with allocated

	VaoLayout layout;
	layout
		.for_buffer(allocated1.name)
			.use_as(GL_ARRAY_BUFFER)
				.bind(0, 3, 1, false)
			.use_as(GL_ELEMENT_ARRAY_BUFFER)
		.for_buffer(allocated2.name)
			.use_as(GL_ARRAY_BUFFER)
				.bind(1, 2, 1, false)
		.for_buffer(allocated3.name)
			.use_as(GL_ARRAY_BUFFER)
				.bind(2, 4, 2, false)
	;

	unsigned int vao = vao_manager->get_vao_for(layout);

	// Register vao as material? It 

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
