#include "GM/Framework/Components/FinalPass.h"

#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"
#include "GM/Core/Utilities/ShaderFactory.h"
#include "GM/Core/Utilities/ShaderSource.h"
#include "GM/Core/Utilities/UniformOperations.h"
#include "GM/Core/GL/VertexArrayObject.h"

namespace GM {
	namespace Framework {

		FinalPass::FinalPass()
		: resolution(1,1)
		{
		}

		FinalPass::~FinalPass()
		{
		}

		void FinalPass::build() {
			typedef std::string s;
			Core::ShaderSource vertex(
				"vertex",
				s("#version 330\n") +
				s("out vec2 tex_coord;\n") +
				s("const vec2 quadVertices[4] = vec2[](\n") + 
				s("    vec2(-1.0, -1.0),\n") +
				s("    vec2(1.0, -1.0),\n") +
				s("    vec2(-1.0, 1.0),\n") +
				s("    vec2(1.0, 1.0)\n") +
				s(");\n") +
				s("void main() {\n") +
				s("    vec2 vertex = quadVertices[gl_VertexID];\n") +
				s("    gl_Position = vec4(vertex, 0.0, 1.0);\n") +
				s("    tex_coord = vec2(0.5 * vertex.x + 0.5, 0.5 * vertex.y + 0.5);\n") +
				s("}\n")
				,
				GL_VERTEX_SHADER
			);

			Core::ShaderSource fragment(
				"fragment",
				s("#version 330\n") +
				s("#define DIFF_TEX 0\n") + 
				s("in vec2 tex_coord;\n") +
				s("layout(location = DIFF_TEX) out vec4 colour;\n") +
				s("uniform sampler2D tex;\n") + 
				s("void main() {\n") +
				s("    vec4 tex_colour = texture2D(tex, tex_coord);\n") +
				s("    colour = tex_colour;\n") +
				s("}\n")
				,
				GL_FRAGMENT_SHADER
			);
			
			int in_width = 0;
			int in_height = 0;

			input_texture->bind();
			glGetTexLevelParameteriv(input_texture->get_type(), 0, GL_TEXTURE_WIDTH, &in_width);
			glGetTexLevelParameteriv(input_texture->get_type(), 0, GL_TEXTURE_HEIGHT, &in_height);
			input_texture->unbind();

			resolution = glm::vec2(in_width, in_height);

			// Move shader generation until where it is needed
			shader = Core::ShaderFactory::make_program({vertex, fragment});
			tex_uniform = shader->get_uniform_info("tex").location;

			vao = std::make_shared<Core::VertexArrayObject>();
		}

		void FinalPass::pass(RenderSystem & /*render_system*/)
		{
			vao->bind();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_BACK);
			glViewportIndexedf(0, 0, 0, resolution.x, resolution.y);
			glClearColor(0,0,0,0);
			glClearDepth(1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			input_texture->bind();
			Core::update_uniform(shader->get_handle(), tex_uniform, 0);

			shader->bind();

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// Unnecessary?
			vao->unbind();
		}

		void FinalPass::set_input_texture(const Core::TexturePtr &new_input_texture)
		{
			input_texture = new_input_texture;
		}

		void FinalPass::set_output_texture(const Core::TexturePtr &)
		{
		}

		std::string FinalPass::get_type() const {
			return "FinalPass";
		}

	} // namespace Framework
} // namespace GM
