#include <cstdlib>
#include <GM/Core/Utilities/ShaderConstants.h>
#include <GM/Core/Utilities/VaoLayout.h>
#include <GM/Framework/Managers/BufferManager.h>
#include <GM/Framework/Managers/VaoManager.h>

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

bool mainTest() {
	auto app = Main::create_with_gl_version("MultipleBufferVaoTest", 3, 3);

	auto buffer_manager = app->get_buffer_manager();
	auto vao_manager = app->get_vao_manager();

	struct MyVertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
	};

	struct MyInstance
	{
		glm::vec3 pos;
	};

	VertexArrayObjectPtr vao1(nullptr), vao2(nullptr), vao3(nullptr);

	// Setup vao1
	{
		std::vector<MyVertex> data(100, {glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,1.f,0.f)});
		std::vector<unsigned short> indices(3*data.size(), 0);
		std::vector<MyInstance> instances(2000, {glm::vec3(0.f, 0.f, 0.f)});

		auto vertex_buffer = buffer_manager->allocate(data.size()*sizeof(MyVertex), sizeof(MyVertex));
		auto index_buffer = buffer_manager->allocate(indices.size()*sizeof(unsigned short), sizeof(unsigned short));
		auto instance_buffer = buffer_manager->allocate(instances.size()*sizeof(MyInstance), sizeof(MyInstance));

		VaoLayout layout1;
		layout1
			.for_buffer(vertex_buffer.buffer)
				.use_as(GL_ARRAY_BUFFER)
					.bind(Core::ShaderConstants::Position, 3, GL_FLOAT, false, sizeof(MyVertex))
					.bind(Core::ShaderConstants::Normal, 3, GL_FLOAT, false, sizeof(MyVertex), offsetof(MyVertex, normal))
			.for_buffer(index_buffer.buffer)
				.use_as(GL_ELEMENT_ARRAY_BUFFER)
			.for_buffer(instance_buffer.buffer)
				.use_as(GL_ARRAY_BUFFER)
					.bind(Core::ShaderConstants::Offset, 3, GL_FLOAT, false, sizeof(MyInstance), 0, 1)
		;

		vao1 = vao_manager->get_vao_for(layout1);
	}

	// Setup vao3
	{
		std::vector<MyVertex> data(10, {glm::vec3(0.f,0.f,1.f), glm::vec3(1.f,0.f,0.f)});
		std::vector<unsigned short> indices(3*data.size(), 0);

		auto vertex_buffer = buffer_manager->allocate<MyVertex>(data.size());
		auto index_buffer = buffer_manager->allocate<unsigned short>(indices.size());

		VaoLayout layout3; // Almost same as vao1 and vao2, except for no instances 
		layout3
			.for_buffer(vertex_buffer.buffer)
				.use_as(GL_ARRAY_BUFFER)
					.bind(Core::ShaderConstants::Normal, 3, GL_FLOAT, false, sizeof(MyVertex), offsetof(MyVertex, normal))
					.bind(Core::ShaderConstants::Position, 3, GL_FLOAT, false, sizeof(MyVertex))
			.for_buffer(index_buffer.buffer)
				.use_as(GL_ELEMENT_ARRAY_BUFFER)
		;

		vao3 = vao_manager->get_vao_for(layout3);
	}

	// Setup vao2
	{
		std::vector<MyVertex> data(10, {glm::vec3(0.f,0.f,1.f), glm::vec3(1.f,0.f,0.f)});
		std::vector<unsigned short> indices(3*data.size(), 0);
		std::vector<MyInstance> instances(5, {glm::vec3(0.f, 1.f, 0.f)});

		auto vertex_buffer = buffer_manager->allocate(data.size()*sizeof(MyVertex), sizeof(MyVertex));
		auto index_buffer = buffer_manager->allocate(indices.size()*sizeof(unsigned short), sizeof(unsigned short));
		auto instance_buffer = buffer_manager->allocate<MyInstance>(instances.size());

		VaoLayout layout2; // Slightly shuffled layout, but should be equal to vao1
		layout2
			.for_buffer(instance_buffer.buffer)
				.use_as(GL_ARRAY_BUFFER)
					.bind<glm::vec3>(Core::ShaderConstants::Offset, sizeof(MyInstance), 0, 1)
			.for_buffer(vertex_buffer.buffer) // VaoLayout should support BufferAllocation?
				.use_as(GL_ARRAY_BUFFER)
					.bind_interleaved(VaoArg<glm::vec3>(Core::ShaderConstants::Position), VaoArg<glm::vec3>(Core::ShaderConstants::Normal))
			.for_buffer(index_buffer.buffer)
				.use_as(GL_ELEMENT_ARRAY_BUFFER)
		;

		vao2 = vao_manager->get_vao_for(layout2);
	}

	if (vao1 == nullptr || vao2 == nullptr || vao1 != vao2)
	{
		throw clan::Exception("Expected vao1 and vao2 to be equal");
	}
	else if (vao3 == nullptr || vao1 == vao3 || vao2 == vao3)
	{
		throw clan::Exception("Expected vao3 to be different from vao1 and vao2");
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
