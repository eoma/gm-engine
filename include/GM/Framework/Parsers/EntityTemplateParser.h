#pragma once

#include "../Managers/TemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class EntityTemplateParser {
		public:
			EntityTemplateParser();

			static void parse_templates(const std::string &data, std::function<void(const TemplateManager::Template &)> func);
		};
		typedef std::shared_ptr<EntityTemplateParser> EntityTemplateParserPtr;
	}
}
