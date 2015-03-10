#include "GM/Framework/Components/StandardPass.h"

#include "GM/Framework/Entity.h"
#include "GM/Framework/RenderLayers.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Core/GL/FramebufferObject.h"
#include "GM/Core/GL/RenderbufferObject.h"
#include "GM/Core/GL/Texture.h"

#include "ClanLib/core.h"

namespace GM
{
	namespace Framework
	{

		StandardPass::StandardPass(const EntityPtr &owner, const std::string &name)
		: Component<StandardPass>(owner, name)
		, camera(nullptr)
		, framebuffer(nullptr)
		, resolution(1,1)
		{
		}

		StandardPass::~StandardPass()
		{
		}

		void StandardPass::initialize()
		{
			if (owner->has_component<Camera>())
			{
				camera = owner->get_component<Camera>().get();
			}
			else
			{
				throw clan::Exception(clan::string_format(
					"%1 requires that a camera exists on entity %2",
					get_type(), owner->get_name()));
			}
		}

		void StandardPass::set_output_texture(const Core::TexturePtr &new_output_texture)
		{
			output_texture = new_output_texture;
		}

		void StandardPass::build()
		{
			if (framebuffer == nullptr)
			{
				framebuffer = std::make_shared<Core::FramebufferObject>();
			}

			int out_width = 0;
			int out_height = 0;

			output_texture->bind();
			glGetTexLevelParameteriv(output_texture->get_type(), 0, GL_TEXTURE_WIDTH, &out_width);
			glGetTexLevelParameteriv(output_texture->get_type(), 0, GL_TEXTURE_HEIGHT, &out_height);
			output_texture->unbind();

			resolution = glm::vec2(out_width, out_height);

			framebuffer->bind();
			framebuffer->add(GL_COLOR_ATTACHMENT0, output_texture);
			framebuffer->add(GL_DEPTH_ATTACHMENT, std::make_shared<Core::RenderbufferObject>(GL_DEPTH_COMPONENT32F, out_width, out_height));
			framebuffer->set_draw_buffer_as_map({{0, GL_COLOR_ATTACHMENT0}});

			glClearDepth(1.0);
			glClearColor(0.0, 0.0, 0.0, 0.0);

			framebuffer->check();
			framebuffer->unbind();
		};

		void StandardPass::pass(RenderSystem &render_system)
		{
			framebuffer->bind();
			glViewportIndexedf(0, 0, 0, resolution.x, resolution.y);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			render_system.pass(*camera, "standard", RenderLayers::MESH_OPAQUE);

			framebuffer->unbind();
		}

	} // Framework
} // GM
