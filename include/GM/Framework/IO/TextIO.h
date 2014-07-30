#pragma once

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class TextIO {
		public:
			TextIO();

			static std::string load_contents(const std::string &filename);
		};
		typedef std::shared_ptr<TextIO> TextIOPtr;
	}
}
