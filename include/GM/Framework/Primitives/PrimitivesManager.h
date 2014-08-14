#pragma once

#include "IPrimitive.h"

#include <vector>

namespace GM {
	namespace Framework {
		class PrimitivesManager {
		public:
			PrimitivesManager(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager);

			void add(IPrimitivePtr primitive);
			MeshPtr create(const std::string &name);

		private:
			BufferManagerPtr buffer_manager;
			VaoManagerPtr vao_manager;

			std::vector<IPrimitivePtr> sleeping_primitives;
			std::vector<IPrimitivePtr> active_primitives;
		};
	}
}