#pragma once

#include "IPrimitive.h"

namespace GM {
    namespace Framework {
        class IcosahedronPrimitive : public IPrimitive {
        public:
            IcosahedronPrimitive();
            MeshPtr create(const BufferManagerPtr &buffer_manager, const VaoManagerPtr &vao_manager) override;
        };
        typedef std::shared_ptr<IcosahedronPrimitive> IcosahedronPrimitivePtr;
    }
}