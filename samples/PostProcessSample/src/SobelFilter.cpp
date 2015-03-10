#include "SobelFilter.h"

#include "GM/Core/Utilities/ShaderSource.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Core/Utilities/TextureFactory.h"
#include "GM/Core/Utilities/UniformOperations.h"
#include "GM/Core/GL/VertexArrayObject.h"
#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"

using namespace GM;

SobelFilter::SobelFilter(const Framework::EntityPtr &owner, const std::string &name)
: Framework::Component<SobelFilter>(owner, name)
, satisfied_and_ready(false)
{
}

SobelFilter::~SobelFilter ()
{
}

void SobelFilter::pass(Framework::RenderSystem &/*render_system*/)
{
	if (!satisfied_and_ready)
	{
		build();
	}

	vao->bind();
	shader->bind();

	fbo_first_pass.bind();
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	input_texture->bind();

	Core::update_uniform(shader->get_handle(), input_texture_uniform, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// We're done, clean up
	fbo_first_pass.unbind();
	shader->unbind();
	vao->unbind();
}

void SobelFilter::set_input_texture(const Core::TexturePtr &new_input_texture)
{
	satisfied_and_ready = false;
	input_texture = new_input_texture;
}

void SobelFilter::set_output_texture(const Core::TexturePtr &new_output_texture)
{
	satisfied_and_ready = false;
	output_texture = new_output_texture;
}

void SobelFilter::build()
{
	if (input_texture == nullptr || output_texture == nullptr)
	{
		return;
	}

	build_fbos();
	construct_shader();

	satisfied_and_ready = true;
}

void SobelFilter::construct_shader()
{
	typedef std::string s;
		
	Core::ShaderSource vertex(
		"vertex",
		"#version 330\n"
		"out vec2 tex_coord;\n"
		"const vec2 quadVertices[4] = vec2[](\n" 
		"    vec2(-1.0, -1.0),\n"
		"    vec2(1.0, -1.0),\n"
		"    vec2(-1.0, 1.0),\n"
		"    vec2(1.0, 1.0)\n"
		");\n"
		"void main() {\n"
		"    vec2 vertex = quadVertices[gl_VertexID];\n"
		"    gl_Position = vec4(vertex, 0.0, 1.0);\n"
		"    tex_coord = vec2(0.5 * vertex.x + 0.5, 0.5 * vertex.y + 0.5);\n"
		"}\n"
		,
		GL_VERTEX_SHADER
	);

	Core::ShaderSource fragment("fragment",
		"#version 330\n"
		"#define DIFF_TEX 0\n"
		"#define PI 3.141592653589793238462643383279\n"
		"\n"
		"in vec2 tex_coord;\n"
		"layout(location = DIFF_TEX) out vec4 colour;\n"
		"\n"
		"uniform sampler2D tex;\n"
		"uniform vec2 Resolution = vec2(800,640);\n"
		"\n"
		"void main() {\n"
		"    vec2 step = 1.0/Resolution;\n"
		"    vec2 tc = tex_coord;\n"
		"\n"
		"    float topLeft = length(texture2D(tex, tc + vec2(-step.x, step.y)).rgb);\n"
		"    float topMiddle = length(texture2D(tex, tc + vec2(0.0, step.y)).rgb);\n"
		"    float topRight = length(texture2D(tex, tc + vec2(step.x, step.y)).rgb);\n"
		"\n"
		"    float centerLeft = length(texture2D(tex, tc + vec2(-step.x, 0.0)).rgb);\n"
		"    float centerRight = length(texture2D(tex, tc + vec2(step.x, 0.0)).rgb);\n"
		"\n"
		"    float bottomLeft = length(texture2D(tex, tc + vec2(-step.x, -step.y)).rgb);\n"
		"    float bottomMiddle = length(texture2D(tex, tc + vec2(0.0, -step.y)).rgb);\n"
		"    float bottomRight = length(texture2D(tex, tc + vec2(step.x, -step.y)).rgb);\n"
		"\n"
		"    float x = (topRight + 2.0*centerRight + bottomRight) - (topLeft + 2.0*centerLeft + bottomLeft);\n"
		"    float y = (bottomLeft + 2.0*bottomMiddle + bottomRight) - (topLeft + 2.0*topMiddle + topRight);\n"
		"    float gradient = sqrt(x*x + y*y);\n"
		"    colour = vec4(gradient, gradient, gradient, 1.0);\n"
		"}\n"
		,
		GL_FRAGMENT_SHADER
	);
			
	shader = Core::ShaderFactory::make_program({vertex, fragment});
	input_texture_uniform = shader->get_uniform_info("tex").location;

	vao = std::make_shared<Core::VertexArrayObject>();
}


void SobelFilter::build_fbos()
{
	// First pass fbo
	fbo_first_pass.bind();
	fbo_first_pass.set_draw_buffer_as_map({{0, GL_COLOR_ATTACHMENT0}});
	fbo_first_pass.add(GL_COLOR_ATTACHMENT0, output_texture);
	fbo_first_pass.check();

	glClearColor(0.0, 0.0, 0.0, 0.0);

	fbo_first_pass.unbind();
}
