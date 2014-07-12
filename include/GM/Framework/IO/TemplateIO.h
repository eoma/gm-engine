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

			static void load_templates(const std::string template_filename, std::function<void(const TemplateManager::Template &)> func);
		};
		typedef std::shared_ptr<TemplateIO> TemplateIOPtr;
	}
}
