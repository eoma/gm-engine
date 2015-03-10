#pragma once

#include "GM/Framework/Components/IRenderPassComponent.h"

namespace GM {
	namespace Core {
		class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
		class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
	};

	namespace Framework {

		/**
 		 * A simple render pass to render a texture to fullscreen
 		 * Only Camera should instantiate this
 		 */
		class FinalPass : public IRenderPassComponent
		{
		public:
			FinalPass();
			virtual ~FinalPass();

			std::string get_type() const override;

			void build() override;

			void pass(RenderSystem &render_system) override;

			bool uses_render_texture_from_camera() const override { return true; };
			void set_input_texture(const Core::TexturePtr &input_texture) override;
			// Not used
			void set_output_texture(const Core::TexturePtr &) override;

		private:
			Core::TexturePtr input_texture;
			Core::ShaderPtr shader;
			Core::VertexArrayObjectPtr vao;

			int tex_uniform;

			glm::vec2 resolution;
		};

		typedef std::shared_ptr<FinalPass> FinalPassPtr;
	} // namespace Framework
} // namespace GM
