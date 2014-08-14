#pragma once

#include "IPrimitive.h"

namespace GM {
	namespace Framework {
		class SkyboxPrimitive : public IPrimitive {
		public:
			SkyboxPrimitive();
			MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
		};
		typedef std::shared_ptr<SkyboxPrimitive> SkyboxPrimitivePtr;
	}
}