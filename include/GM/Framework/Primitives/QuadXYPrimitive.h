#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class QuadXYPrimitive : public IPrimitive {
		public:
			QuadXYPrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<QuadXYPrimitive> QuadXYPrimitivePtr;
	}
}