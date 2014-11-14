#pragma once

namespace GM {
	namespace Framework {
		struct RenderLayers {
			enum Value {
				MESH_OPAQUE = (1<<0),
				MESH_TRANSPARENT = (1<<1)
			};
		};
	}
}
