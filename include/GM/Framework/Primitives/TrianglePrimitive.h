#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class TrianglePrimitive : public IPrimitive {
		public:
			TrianglePrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<TrianglePrimitive> TrianglePrimitivePtr;
	}
}