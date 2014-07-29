#pragma once

#include <map>
#include <memory>
#include <string>

namespace GM {
	namespace Framework {

		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;

		class MeshManager {
		public:
			MeshManager();
			~MeshManager();

			bool contains(const std::string &name) const;

			void add(const std::string &name, const MeshPtr &mesh);

			MeshPtr get(const std::string &name) const;

			//MeshPtr get_or_create(const std::string &name);
		public:

		private:
			std::map<std::string, MeshPtr> meshes;
		};

		typedef std::shared_ptr<MeshManager> MeshManagerPtr;

	} // namespace Framework
} // namespace GM
