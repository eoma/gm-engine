#pragma once

#include <ClanLib/core.h>

#include <memory>
#include <string>

namespace GM {
	namespace Framework {
		class Entity; typedef std::shared_ptr<Entity> EntityPtr;

		class ComponentSerializer
		{
		public:
			ComponentSerializer() {}

			virtual void create_and_add_component(const EntityPtr &owner, const std::string &type, const std::string &name);

			clan::Signal<void(const EntityPtr &/*owner*/, const std::string &/*type*/, const std::string &/*name*/)> sig_create_component;
		};

		typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;
	}
}
