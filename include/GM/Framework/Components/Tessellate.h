#pragma once

#include "../Totem/Component.h"
#include "../DefinitionsComponentNames.h"

#include <ClanLib/core.h>

#include <glm/glm.hpp>

#include <memory>

namespace GM
{
	namespace Framework
	{
		class Tessellate; typedef std::shared_ptr<Tessellate> TessellatePtr;
		class Entity; typedef std::shared_ptr<Entity> EntityPtr;
		class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

		/**
		 * The tessellate component sets certain parameters relevant for tesselation
		 */
		class Tessellate : public Component<Tessellate>
		{
		public:
			Tessellate(const EntityPtr &owner, const RenderSystemPtr &render_system, const std::string &name = std::string());
			virtual ~Tessellate();

			std::string get_type() const override;
			static std::string get_static_type() { return GM_COMPONENT_TESSELLATE; };

			/**
			 * Get inner tesselation level.
			 *
			 * @return inner tesselation level
			 */
			int get_tess_level_inner() const;

			/**
			 * Get outer tesselation level
			 *
			 * @return outer tesselation level
			 */
			int get_tess_level_outer() const;

			/**
			 * Get number of vertices per patch.
			 *
			 * @return number of vertices per patch
			 */
			int get_patch_vertices() const;

		private:
			clan::SlotContainer slots;

			Property<int> tess_level_inner;
			Property<int> tess_level_outer;
			Property<int> patch_vertices;
		};
	}
}
