#include "GM/Framework/IO/AssimpMeshIO.h"
#include <assimp/Importer.hpp>
#include <ClanLib/core.h>

#include <cctype>
#include <algorithm>

namespace GM {
namespace Framework {

AssimpMeshIO::AssimpMeshIO()
{
}

AssimpMeshIO::~AssimpMeshIO()
{
}

Core::VaoLayout AssimpMeshIO::load(const std::string &file_name)
{
	//clan::DataBuffer buffer = clan::File::read_bytes(file_name);
	//return load(buffer.get_data<unsigned char>(), buffer.get_size());
	return Core::VaoLayout();
}

} // namespace Framework
} // namespace GM
