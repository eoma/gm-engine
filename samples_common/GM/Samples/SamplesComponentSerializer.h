#pragma once

#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"

namespace GM {
	namespace Samples {
		class SamplesComponentSerializer {
		public:
			SamplesComponentSerializer(const Application::MainPtr &app);
		};
		typedef std::shared_ptr<SamplesComponentSerializer> SamplesComponentSerializerPtr;
	}
}
