#pragma once

class PropertySerializer
{
public:
	enum PropertyType
	{
		TYPE_UNSIGNED_INT = 1,
		TYPE_INT = 2,
		TYPE_FLOAT = 3,
		TYPE_BOOL = 4,
		TYPE_DOUBLE = 5,
		TYPE_CHAR = 6,
		TYPE_STRING = 100,
		TYPE_CL_VEC2I = 101,			// x y
		TYPE_CL_VEC3F = 102,			// x y z
		TYPE_CL_COLORF = 103			// r g b a
	};

public:
	// Factories
	static void create_and_add_property(Totem::PropertyContainer<> &container, int type_id, const std::string &name, const std::string &value);

	// Type Identification
	static PropertyType get_property_type(std::shared_ptr<Totem::IProperty> property);
	static PropertyType get_property_type(Totem::IProperty *property);
	static PropertyType get_property_type(const std::string &property_type);

	static std::string property_value_to_string(std::shared_ptr<Totem::IProperty> property);
	static std::string property_value_to_string(Totem::IProperty *property);

	template<typename Type> 
	static void from_string(const std::string &value, Type &result);
	static void from_string(const std::string &value, unsigned int &result);
	static void from_string(const std::string &value, int &result);
	static void from_string(const std::string &value, float &result);
	static void from_string(const std::string &value, double &result);
	static void from_string(const std::string &value, bool &result);
	static void from_string(const std::string &value, char &result);
	static void from_string(const std::string &value, std::string &result);
	static void from_string(const std::string &value, clan::Vec2i &result);
	static void from_string(const std::string &value, clan::Vec3f &result);
	static void from_string(const std::string &value, clan::Colorf &result);
};
