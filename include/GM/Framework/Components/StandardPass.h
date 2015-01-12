#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"
#include "IRenderPassComponent.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM {
	namespace Framework {

		class Camera; typedef std::shared_ptr<Camera> CameraPtr;
		class Entity; typedef std::shared_ptr<Entity> EntityPtr;

		/**
		 * Will make the camera a shadow camera, it will only make shadows.
		 * You only need to make _one_ camera that is supposed work as a shadow camera
		 */
		class StandardPass : public Component<StandardPass>, public IRenderPassComponent {
		public:
			StandardPass(const EntityPtr &owner, const std::string &name = std::string());
			virtual ~StandardPass();

			std::string get_type() const override { return get_static_type(); };
			static std::string get_static_type() { return GM_COMPONENT_STANDARD_PASS; };

			void initialize() override;

			void build() override;
			void pass(RenderSystem * const render_system) override;

			bool uses_render_texture_from_camera() const override { return false; }
			void set_input_texture(const Core::TexturePtr &) override {}
			void set_output_texture(const Core::TexturePtr &) override {}

		private:
			Camera *camera;
		};

	} // namespace Framework
} // namespace GM
