#pragma once

#include "MeshTemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class MeshTemplateParser {
		public:
			MeshTemplateParser();

			static void parse_templates(const std::string &data, std::function<void(const MeshTemplateManager::Template &)> func);
		};
		typedef std::shared_ptr<MeshTemplateParser> MeshTemplateParserPtr;
	}
}
