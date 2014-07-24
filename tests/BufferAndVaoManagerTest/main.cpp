#include <cstdlib>
#include <GM/Core/Utilities/BufferOperations.h>
#include <GM/Core/Utilities/VaoLayout.h>
#include <GM/Framework/Managers/BufferManager.h>
#include <GM/Framework/Managers/VaoManager.h>
#include <GM/Framework/Utilities/Tools.h>

#include <GM/Application/Main.h>

#include <glm/glm.hpp>

#include <GL/gl3w.h>

#include <cstddef>
#include <iostream>
#include <memory>

using namespace GM;
using namespace Core;
using namespace Framework;
using namespace Application;

#define POSITION 0
#define NORMAL 1
#define INDEX 2
#define INSTANCE_POSITION 3
#define INSTANCE_SCALE 4

bool mainTest() {
	auto app = Main::create_with_gl_version("BufferAndVaoManagerTest", 3, 3);
	auto buffer_manager = app->get_buffer_manager();
	auto vao_manager = app->get_vao_manager();

	struct MyInterlacedVertex
	{
		// prefer variable types with a size that are dividable by 4 bytes
		glm::vec3 pos;
		glm::vec3 normal;
	};

	std::vector<MyInterlacedVertex> data(100, {glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,1.f,0.f)});

	std::vector<unsigned short> indices(3*data.size(), 0);

	struct MyInstance
	{
		// prefer variable types with a size that are dividable by 4 bytes (not bool or char or short)
		glm::vec3 pos;
		float scale;
	};

	std::vector<MyInstance> instances(2000, {glm::vec3(0.f, 0.f, 0.f), 1.f});

	// Buffer should be allocated before we start our VAO definition, 
	auto vertex_buffer = buffer_manager->allocate<MyInterlacedVertex>(data.size());
	vertex_buffer.upload(data);
	
	auto index_buffer = buffer_manager->allocate_and_upload(indices);

	auto instance_buffer = buffer_manager->allocate(total_size(instances), BufferManager::UNIQUE_BUFFER);
	instance_buffer.upload(instances);
	

	//auto all_buffer = buffer_manager->allocate(
	//	total_sizeof(data, indices, instances)
	//);

	// Do stuff with all_buffer
	//Core::BufferOperations::upload(data, indices, instances);

	VaoLayout layout;
	layout
		.for_buffer(vertex_buffer) // VaoLayout supports BufferAllocation
			.use_as(GL_ARRAY_BUFFER)
				.bind(POSITION, 3, GL_FLOAT, false, sizeof(MyInterlacedVertex))
				// offsetof will be unsafe if using dynamic data structures (like a struct with std::vector)
				// offsetof should only be used on POD structs (not classes, like struct with constructor)
				.bind(NORMAL, 3, GL_FLOAT, false, sizeof(MyInterlacedVertex), offsetof(MyInterlacedVertex, normal))
		.for_buffer(index_buffer.buffer)
			.use_as(GL_ELEMENT_ARRAY_BUFFER)
		.for_buffer(instance_buffer)
			.use_as(GL_ARRAY_BUFFER)
				.bind(INSTANCE_POSITION, 3, GL_FLOAT, false, sizeof(MyInstance), 0, 1)
				.bind(INSTANCE_SCALE, 1, GL_FLOAT, false, sizeof(MyInstance), offsetof(MyInstance, scale), 1)
				// maybe a convenience method
				//.bind_interlaced(divisor, arg<glm::vec3>(INSTANCE_POSITION), arg<float>(INSTANCE_SCALE))
	;

	auto vao = vao_manager->get_vao_for(layout);

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
