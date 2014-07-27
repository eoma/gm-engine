#pragma once

#include <memory>

namespace GM {
	namespace Framework {

		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;

		class MeshSystem {
		public:
			MeshSystem();
			~MeshSystem();

		public:

		private:
		};

		typedef std::shared_ptr<MeshSystem> MeshSystemPtr;

	} // namespace Framework
} // namespace GM
