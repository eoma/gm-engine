#pragma once

#include "ShaderTemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class ShaderTemplateParser {
		public:
			ShaderTemplateParser();

			static void parse_templates(const std::string &data, std::function<void(const ShaderTemplateManager::Template &)> func);
		};
		typedef std::shared_ptr<ShaderTemplateParser> ShaderTemplateParserPtr;
	}
}
