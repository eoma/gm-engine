#pragma once

#include "TextureTemplateManager.h"

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class TextureTemplateParser {
		public:
			TextureTemplateParser();

			static void parse_templates(const std::string &data, std::function<void(const TextureTemplateManager::Template &)> func);
		};
		typedef std::shared_ptr<TextureTemplateParser> TextureTemplateParserPtr;
	}
}
