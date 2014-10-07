#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Core/Utilities/VaoFactory.h"

#include <GL/gl3w.h>

namespace GM {
namespace Framework {

using namespace Core;

VaoManager::VaoManager()
: layouts_to_vaos()
, vaos_to_layouts()
{
}

VaoManager::~VaoManager()
{
}

Core::VertexArrayObjectPtr VaoManager::get_vao_for(const VaoLayout &layout)
{
	auto iter = layouts_to_vaos.find(layout);

	if (iter == layouts_to_vaos.end())
	{
		auto vao = build_vao(layout);
		iter = layouts_to_vaos.insert(std::make_pair(layout, vao)).first;
		vaos_to_layouts.insert(std::make_pair(vao, layout));
	}

	return iter->second;
}

Core::VaoLayout VaoManager::get_vao_layout_for(const Core::VertexArrayObjectPtr &vao) const
{
	auto iter = vaos_to_layouts.find(vao);
	if (iter != vaos_to_layouts.end())
	{
		return iter->second;
	}
	else
	{
		return Core::VaoLayout();
	}
}

Core::VertexArrayObjectPtr VaoManager::build_vao(const VaoLayout &layout)
{
	return Core::VaoFactory::create(layout);
}

} // namespace Framework
} // namespace GM
