#include "GM/Framework/Primitives/PrimitivesManager.h"
#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;

PrimitivesManager::PrimitivesManager(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager)
	: buffer_manager(buffer_manager), vao_manager(vao_manager), sleeping_primitives(), active_primitives()
{
}

void PrimitivesManager::add(IPrimitivePtr primitive) {
	auto iter = std::find(sleeping_primitives.begin(), sleeping_primitives.end(), primitive);
	if (iter != sleeping_primitives.end()) {
		throw clan::Exception("Tried to add primitive type (%1) more than once.");
	}

	iter = std::find(active_primitives.begin(), active_primitives.end(), primitive);
	if (iter != active_primitives.end()) {
		throw clan::Exception(clan::string_format("Tried to add primitive type (%1) more than once.", primitive->get_name()));
	}

	sleeping_primitives.push_back(primitive);
}

MeshPtr PrimitivesManager::create(const std::string &name) {
	for (auto active : active_primitives) {
		if (active->get_name() == name) {
			throw clan::Exception(clan::string_format("The primitive type (%1) has already been created and activated. We don't want to duplicate!", name));
		}
	}

	for (auto iter = sleeping_primitives.begin(); iter != sleeping_primitives.end(); ++iter) {
		auto sleeping = (*iter);
		if (sleeping->get_name() == name) {
			active_primitives.push_back(sleeping);
			sleeping_primitives.erase(iter);
			return sleeping->create(buffer_manager, vao_manager);
		}
	}

	return nullptr;
}