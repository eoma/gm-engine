#pragma once

#include <memory>
#include <string>

namespace GM {
namespace Core {
	namespace ShaderConstants
	{
		enum Constants {
			//These are equivalent to the defines in shader
			Diffuse = 0,
			Position = 1,
			Normal = 2,
			TexCoord = 3,
			Tangent = 4,
			Bitangent = 5,
			Offset = 6,
			InstancedNormal = 7,
			FragColor = 0
		};
	}
}
}
