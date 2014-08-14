#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class CubePrimitive : public IPrimitive {
		public:
			CubePrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<CubePrimitive> CubePrimitivePtr;
	}
}