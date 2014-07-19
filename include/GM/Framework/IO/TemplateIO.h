#pragma once

#include "../Managers/TemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class TemplateIO {
		public:
			TemplateIO();

			static std::string load_contents(const std::string &template_filename);
		};
		typedef std::shared_ptr<TemplateIO> TemplateIOPtr;
	}
}
