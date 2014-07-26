#include "GM/Framework/Managers/VaoManager.h"

#include "GM/Core/Utilities/VaoFactory.h"

#include <GL/gl3w.h>

namespace GM {
namespace Framework {

using namespace Core;

VaoManager::VaoManager()
: vaos()
{
}

VaoManager::~VaoManager()
{
}

Core::VertexArrayObjectPtr VaoManager::get_vao_for(const VaoLayout &layout)
{
	auto iter = vaos.find(layout);

	if (iter == vaos.end())
	{
		auto vao = build_vao(layout);
		iter = vaos.insert(std::make_pair(layout, vao)).first;
	}

	return iter->second;
}

Core::VertexArrayObjectPtr VaoManager::build_vao(const VaoLayout &layout)
{
	return Core::VaoFactory::create(layout);
}

} // namespace Framework
} // namespace GM
