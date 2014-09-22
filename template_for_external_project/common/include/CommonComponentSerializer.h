#pragma once

#include "GM/Application/Main.h"
#include "GM/Framework/Framework.h"

namespace GM {
	namespace Common {
		class CommonComponentSerializer {
		public:
			CommonComponentSerializer(const Application::MainPtr &app);
			void create_and_add_component(const Framework::EntityPtr &owner, const std::string &type, const std::string &/*name*/);

		private:
			Application::Main *app;
			clan::SlotContainer slots;
		};
		typedef std::shared_ptr<CommonComponentSerializer> CommonComponentSerializerPtr;
	}
}
