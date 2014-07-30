#include "GM/Framework/IO/AssimpMeshIO.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <ClanLib/core.h>

#include <cctype>
#include <algorithm>

namespace GM {
namespace Framework {

AssimpMeshIO::AssimpMeshIO()
: importer(new Assimp::Importer())
{
}

AssimpMeshIO::~AssimpMeshIO()
{
	delete importer;
}

Core::VaoLayout AssimpMeshIO::load(const std::string &file_name)
{
	auto scene = importer->ReadFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	auto mesh_index = scene->mRootNode->mMeshes[0];
	auto mesh = scene->mMeshes[mesh_index];
	
	//clan::DataBuffer buffer = clan::File::read_bytes(file_name);
	//return load(buffer.get_data<unsigned char>(), buffer.get_size());
	return Core::VaoLayout();
}

} // namespace Framework
} // namespace GM
