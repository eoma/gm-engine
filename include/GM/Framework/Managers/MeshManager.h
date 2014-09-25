#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace GM {
	namespace Framework {

		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;
		class IMeshIO; typedef std::shared_ptr<IMeshIO> IMeshIOPtr;
		class MeshTemplateManager; typedef std::shared_ptr<MeshTemplateManager> MeshTemplateManagerPtr;
		class IPrimitive; typedef std::shared_ptr<IPrimitive> IPrimitivePtr;
		class PrimitivesManager; typedef std::shared_ptr<PrimitivesManager> PrimitivesManagerPtr;
		class BufferManager; typedef std::shared_ptr<BufferManager> BufferManagerPtr;
		class VaoManager; typedef std::shared_ptr<VaoManager> VaoManagerPtr;

		class MeshManager {
		public:
			MeshManager(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager, const IMeshIOPtr &mesh_io);
			~MeshManager();

			// Will replace all texture paths with a single path
			void set_mesh_path(const std::string &mesh_path) { this->mesh_paths = {mesh_path}; }

			// Will replace the current collection of resource paths with a new collection
			void set_mesh_path(const std::vector<std::string> &mesh_paths) { this->mesh_paths = mesh_paths; }

			// Will add a resource path to the existing collection of resource paths
			void add_mesh_path(const std::string &mesh_path) { this->mesh_paths.push_back(mesh_path); }

			bool contains(const std::string &name) const;

			void add(const MeshPtr &mesh);

			MeshPtr get(const std::string &name) const;

			MeshPtr get_or_create(const std::string &name);
			MeshPtr get_or_create(const std::string &name, const std::string &filename, int mesh_index);

			void add_templates(const std::string &template_filename);
			void add_primitive(IPrimitivePtr primitive);
		public:

		private:
			BufferManagerPtr buffer_manager;
			VaoManagerPtr vao_manager;
			MeshTemplateManagerPtr template_manager;
			PrimitivesManagerPtr primitives_manager;

			IMeshIOPtr mesh_io;

			std::vector<std::string> mesh_paths;
			std::map<std::string, MeshPtr> meshes;

		};

		typedef std::shared_ptr<MeshManager> MeshManagerPtr;

	} // namespace Framework
} // namespace GM
