#pragma once

#include <memory>

namespace GM {
	namespace Application {
		class Main; typedef std::shared_ptr<Main> MainPtr;
	}

	namespace Samples {
		class SamplesComponentSerializer {
		public:
			SamplesComponentSerializer(const Application::MainPtr &app);
		};
		typedef std::shared_ptr<SamplesComponentSerializer> SamplesComponentSerializerPtr;
	}
}
