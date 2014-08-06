#pragma once



#include <string>
#include <vector>
#include <memory>

namespace GM {
namespace Framework {
	class BufferManager; typedef std::shared_ptr<BufferManager> BufferManagerPtr;
	class VaoManager; typedef std::shared_ptr<VaoManager> VaoManagerPtr;
	class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;
class IMeshIO {
public:
	virtual ~IMeshIO() {};

	/**
	 * Load mesh from file.
	 */
	virtual MeshPtr load(const std::string &mesh_name, const std::string &file_name, unsigned int mesh_index, const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) = 0;
};

} // namespace Framework
} // namespace GM
