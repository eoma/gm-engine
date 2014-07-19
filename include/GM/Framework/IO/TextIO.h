#pragma once

#include "../Managers/TemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class TextIO {
		public:
			TextIO();

			static std::string load_contents(const std::string &template_filename);
		};
		typedef std::shared_ptr<TextIO> TextIOPtr;
	}
}
