#pragma once

#include <string>
#include <memory>
#include <functional>

namespace GM {
	namespace Framework {

		class ShaderIO {
		public:
			ShaderIO();

			static std::string load_contents(const std::string &file_name);
		};
		typedef std::shared_ptr<ShaderIO> ShaderIOPtr;
	}
}
