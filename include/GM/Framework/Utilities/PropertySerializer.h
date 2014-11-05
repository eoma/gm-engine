#pragma once

#include "../Totem/Totem.h"
#include "../Utilities/Color.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GM {
	namespace Framework {
		class PropertySerializer {
		public:
			enum PropertyType {
				TYPE_UNSIGNED_INT = 1,
				TYPE_INT = 2,
				TYPE_FLOAT = 3,
				TYPE_BOOL = 4,
				TYPE_DOUBLE = 5,
				TYPE_CHAR = 6,
				TYPE_STRING = 100,
				TYPE_VEC2 = 101,			// x y
				TYPE_VEC3 = 102,			// x y z
				TYPE_VEC4 = 103,			// x y z w
				TYPE_QUAT = 104,			// x y z w
				TYPE_COLOR = 105,			// string (0xFFFFFF or 0xFFFFFFFF)
				TYPE_TEXTURE_NAME = 106			// string (texture name)
			};

		public:
			// Factories
			static void create_and_add_property(PropertyContainer<> &container, int type_id, const std::string &name, const std::string &value);

			// Type Identification
			static PropertyType get_property_type(std::shared_ptr<IProperty> property);
			static PropertyType get_property_type(IProperty *property);
			static PropertyType get_property_type(const std::string &property_type);

			static std::string property_value_to_string(std::shared_ptr<IProperty> property);
			static std::string property_value_to_string(IProperty *property);

			template<typename Type>
			static void from_string(const std::string &value, Type &result);
			static void from_string(const std::string &value, unsigned int &result);
			static void from_string(const std::string &value, int &result);
			static void from_string(const std::string &value, float &result);
			static void from_string(const std::string &value, double &result);
			static void from_string(const std::string &value, bool &result);
			static void from_string(const std::string &value, char &result);
			static void from_string(const std::string &value, std::string &result);
			static void from_string(const std::string &value, glm::vec2 &result);
			static void from_string(const std::string &value, glm::vec3 &result);
			static void from_string(const std::string &value, glm::vec4 &result);
			static void from_string(const std::string &value, glm::quat &result);
			static void from_string(const std::string &value, Color &result);
		};
	}
}
