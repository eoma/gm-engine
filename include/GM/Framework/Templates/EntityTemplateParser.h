#pragma once

#include "EntityTemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class EntityTemplateParser {
		public:
			EntityTemplateParser();

			static void parse_templates(const std::string &data, std::function<void(const EntityTemplateManager::Template &)> func);
		};
		typedef std::shared_ptr<EntityTemplateParser> EntityTemplateParserPtr;
	}
}
