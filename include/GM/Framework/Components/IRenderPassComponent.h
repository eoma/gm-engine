#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM {

	namespace Core {
		class Texture; typedef std::shared_ptr<Texture> TexturePtr;
	}

	namespace Framework {

		// Forward declarations
		class RenderSystem;

		/**
		 * Base interface all render pass components must implement
		 */
		class IRenderPassComponent {
		public:
			virtual ~IRenderPassComponent() {};

			/**
			 * Get type name of this component
			 */
			virtual std::string get_type() const = 0;

			/**
			 * Supposed to actually build or setup the render pass component.
			 * This method will be triggered when all inputs and output are set.
			 */
			virtual void build() = 0;

			/**
			 *  Render pass component may do whatever they want, please be kind.
			 *  Render pass components will _always_ be used on a camera.
			 */
			virtual void pass(RenderSystem &render_system) = 0;

			/**
			 * Post process passes should accept an input source and an output source.
			 */
			virtual bool uses_render_texture_from_camera() const = 0;

			/**
			 * Set input texture of this pass. Should only be read from.
			 *
			 * @param input_texture the texture to read from
			 */
			virtual void set_input_texture(const Core::TexturePtr &input_texture) = 0;

			/**
			 * Set output texture of this pass. The pass should write it's final values
			 * in to this texture.
			 *
			 * @param output_texture the texture to write to
			 */
			virtual void set_output_texture(const Core::TexturePtr &output_texture) = 0;
		};

	} // namespace Framework
} // namespace GM
