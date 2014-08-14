#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class QuadPrimitive : public IPrimitive {
		public:
			QuadPrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<QuadPrimitive> QuadPrimitivePtr;
	}
}