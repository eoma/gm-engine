#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"
#include "IRenderPassComponent.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM {
	namespace Core {
		class FramebufferObject; typedef std::shared_ptr<FramebufferObject> FramebufferObjectPtr;
	}

	namespace Framework {

		class Camera; typedef std::shared_ptr<Camera> CameraPtr;
		class Entity; typedef std::shared_ptr<Entity> EntityPtr;

		/**
		 * A simple pass that will render meshes to a texture.
		 */
		class StandardPass : public Component<StandardPass>, public IRenderPassComponent {
		public:
			StandardPass(const EntityPtr &owner, const std::string &name = std::string());
			virtual ~StandardPass();

			std::string get_type() const override { return get_static_type(); };
			static std::string get_static_type() { return GM_COMPONENT_STANDARD_PASS; };

			void initialize() override;

			void build() override;
			void pass(RenderSystem &render_system) override;

			bool uses_render_texture_from_camera() const override { return true; }
			void set_output_texture(const Core::TexturePtr &output_texture) override;

			// NOT USED!
			// The standard pass will only produce texture data
			void set_input_texture(const Core::TexturePtr &) override {}

		private:
			Camera *camera;
			Core::FramebufferObjectPtr framebuffer;
			Core::TexturePtr output_texture;

			glm::vec2 resolution;
		};

	} // namespace Framework
} // namespace GM
