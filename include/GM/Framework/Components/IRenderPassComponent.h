#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM {
	namespace Framework {

		// Forward declarations
		class RenderSystem;

		/**
		 * Base interface all render pass components must implement
		 */
		class IRenderPassComponent {
		public:
			virtual ~IRenderPassComponent() {};

			// Render pass component may do whatever they want, please be kind.
			// Render pass components will _always_ be used on a camera.
			virtual void pass(RenderSystem * const render_system) = 0;
		};

	} // namespace Framework
} // namespace GM
