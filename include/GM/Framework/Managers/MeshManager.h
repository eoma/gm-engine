#pragma once

#include <map>
#include <memory>
#include <string>

namespace GM {
	namespace Framework {

		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;
		class IMeshIO; typedef std::shared_ptr<IMeshIO> IMeshIOPtr;
		class MeshTemplateManager; typedef std::shared_ptr<MeshTemplateManager> MeshTemplateManagerPtr;
		class BufferManager; typedef std::shared_ptr<BufferManager> BufferManagerPtr;
		class VaoManager; typedef std::shared_ptr<VaoManager> VaoManagerPtr;

		class MeshManager {
		public:
			MeshManager(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager, const IMeshIOPtr &mesh_io);
			~MeshManager();

			void set_mesh_path(const std::string &mesh_path) { this->mesh_path = mesh_path; }

			bool contains(const std::string &name) const;

			void add(const std::string &name, const MeshPtr &mesh);

			MeshPtr get(const std::string &name) const;

			MeshPtr get_or_create(const std::string &name);
			MeshPtr get_or_create(const std::string &name, const std::string &filename, int mesh_index);

			void add_templates(const std::string &template_filename);
		public:

		private:
			BufferManagerPtr buffer_manager;
			VaoManagerPtr vao_manager;
			MeshTemplateManagerPtr template_manager;

			IMeshIOPtr mesh_io;

			std::string mesh_path;
			std::map<std::string, MeshPtr> meshes;

		};

		typedef std::shared_ptr<MeshManager> MeshManagerPtr;

	} // namespace Framework
} // namespace GM
