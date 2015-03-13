#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Application/Main.h"

#include "GM/Core/Utilities/Tools.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

using namespace GM;
using namespace Application;

bool mainTest() {
	auto app = Main::create_with_gl_version("ShaderFactoryTest", 3, 3);

	bool update_called = false;

	Core::ShaderPtr shader = nullptr;

	typedef std::string s;

	shader = Core::ShaderFactory::make_program({
		Core::ShaderSource("vertex",
			s("#version 330\n") +
			s("#extension GL_ARB_shading_language_420pack : require\n") +
			s("in vec4 position_ws;\n") +
			s("uniform int non_active_uniform = 0;\n") +
			s("uniform int active_uniform = 1;\n") +
			s("layout(std140) uniform Std140UniformBlock {\n") + 
			s("    mat4 std140_some_matrix;\n") +
			s("    float std140_some_scalar;\n") +
			s("    vec3 std140_some_vector_array[4];\n") +
			s("    vec4 std140_some_vector;\n") +
			s("    float std140_some_scalar2;\n") +
			s("    mat3 std140_some_other_matrix;\n") +
			s("}\n;") +
			s("layout(shared, binding = 10) uniform SharedUniformBlock {\n") + 
			s("    mat4 shared_some_matrix;\n") +
			s("    float shared_some_scalar;\n") +
			s("    vec4 shared_some_vector;\n") +
			s("    vec3 shared_some_vector_array[4];\n") +
			s("    float shared_some_scalar2;\n") +
			s("    mat3 shared_some_other_matrix;\n") +
			s("} instance[3]\n;") +
			s("layout(packed) uniform PackedUniformBlock {\n") + 
			s("    mat4 packed_some_matrix;\n") +
			s("    float packed_some_scalar;\n") +
			s("    vec4 packed_some_vector;\n") +
			s("    vec3 packed_some_vector_array[4];\n") +
			s("    float packed_some_scalar2;\n") +
			s("    mat3 packed_some_other_matrix;\n") +
			s("}\n;") +
			s("struct MyType {\n") + 
			s("    ivec3 something_array[3];\n") +
			s("    float some_float;\n") +
			s("};\n") +
			s("uniform MyType my_type_collection[3];") +
			s("void main() {\n") +
			s("    gl_Position = position_ws + vec4(active_uniform, 0.0, 0.0, 0.0) + std140_some_vector + instance[1].shared_some_vector + packed_some_vector;\n") +
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

	Core::print_shader_info(shader);

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
