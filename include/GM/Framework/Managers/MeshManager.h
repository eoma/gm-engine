#pragma once

#include <memory>

namespace GM {
	namespace Framework {

		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;

		class MeshManager {
		public:
			MeshManager();
			~MeshManager();

		public:

		private:
		};

		typedef std::shared_ptr<MeshManager> MeshManagerPtr;

	} // namespace Framework
} // namespace GM
