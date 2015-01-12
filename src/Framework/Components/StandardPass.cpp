#include "GM/Framework/Components/StandardPass.h"

#include "GM/Framework/Entity.h"
#include "GM/Framework/RenderLayers.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Systems/RenderSystem.h"

#include "ClanLib/core.h"

namespace GM
{
	namespace Framework
	{

		StandardPass::StandardPass(const EntityPtr &owner, const std::string &name)
		: Component<StandardPass>(owner, name)
		, camera(nullptr)
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

		void StandardPass::build() {};

		void StandardPass::pass(RenderSystem *render_system)
		{
			render_system->pass(camera, "standard", RenderLayers::MESH_OPAQUE);
		}

	} // Framework
} // GM
