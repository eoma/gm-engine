#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class FullscreenQuadPrimitive : public IPrimitive {
		public:
			FullscreenQuadPrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<FullscreenQuadPrimitive> FullscreenQuadPrimitivePtr;
	}
}