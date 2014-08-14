#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class QuadXZPrimitive : public IPrimitive {
		public:
			QuadXZPrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<QuadXZPrimitive> QuadXZPrimitivePtr;
	}
}