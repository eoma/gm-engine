
#include <GM/Framework/Utilities/PropertySerializer.h>
#include <GM/Framework/Totem/IProperty.h>

using namespace GM;
using namespace Framework;
using namespace clan;

void PropertySerializer::create_and_add_property(PropertyContainer<> &container, int type_id, const std::string &name, const std::string &value)
{
	switch(type_id)
	{
		case PropertySerializer::TYPE_UNSIGNED_INT:
		{
			unsigned int result;
			from_string(value, result);
			Property<unsigned int> property = container.add<unsigned int>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_INT:
		{
			int result;
			from_string(value, result);
			Property<int> property = container.add<int>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_FLOAT:
		{
			float result;
			from_string(value, result);
			Property<float> property = container.add<float>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_BOOL:
		{
			bool result;
			from_string(value, result);
			Property<bool> property = container.add<bool>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_DOUBLE:
		{
			double result;
			from_string(value, result);
			Property<double> property = container.add<double>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_CHAR:
		{
			char result;
			from_string(value, result);
			Property<char> property = container.add<char>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_STRING:
		{
			std::string result;
			from_string(value, result);
			Property<std::string> property = container.add<std::string>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_VEC2:
		{
			glm::vec2 result;
			from_string(value, result);
			Property<glm::vec2> property = container.add<glm::vec2>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_VEC3:
		{
			glm::vec3 result;
			from_string(value, result);
			Property<glm::vec3> property = container.add<glm::vec3>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_VEC4:
		{
			glm::vec4 result;
			from_string(value, result);
			Property<glm::vec4> property = container.add<glm::vec4>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_QUAT:
		{
			glm::quat result;
			from_string(value, result);
			Property<glm::quat> property = container.add<glm::quat>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_COLOR:
		{
			Color result;
			from_string(value, result);
			Property<Color> property = container.add<Color>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_TEXTURE_NAME:
		{
			std::string result;
			from_string(value, result);
			Property<std::string> property = container.add<std::string>(name, result);
			property.set(result);
			break;
		}
		default:
			throw Exception(string_format("PropertySerializer::create_and_add_property - Unknown property type %1", type_id));
	}
}

PropertySerializer::PropertyType PropertySerializer::get_property_type(const std::string &property_type)
{
	if(StringHelp::compare("unsigned int", property_type, true) == 0)
		return PropertySerializer::TYPE_UNSIGNED_INT;
	if(StringHelp::compare("int", property_type, true) == 0)
		return PropertySerializer::TYPE_INT;
	if(StringHelp::compare("float", property_type, true) == 0)
		return PropertySerializer::TYPE_FLOAT;
	if(StringHelp::compare("bool", property_type, true) == 0 || StringHelp::compare("boolean", property_type, true) == 0)
		return PropertySerializer::TYPE_BOOL;
	if(StringHelp::compare("double", property_type, true) == 0)
		return PropertySerializer::TYPE_DOUBLE;
	if(StringHelp::compare("char", property_type, true) == 0)
		return PropertySerializer::TYPE_CHAR;
	if(StringHelp::compare("string", property_type, true) == 0)
		return PropertySerializer::TYPE_STRING;
	if(StringHelp::compare("vec2", property_type, true) == 0)
		return PropertySerializer::TYPE_VEC2;
	if(StringHelp::compare("vec3", property_type, true) == 0)
		return PropertySerializer::TYPE_VEC3;
	if(StringHelp::compare("vec4", property_type, true) == 0)
		return PropertySerializer::TYPE_VEC4;
	if (StringHelp::compare("quat", property_type, true) == 0)
		return PropertySerializer::TYPE_QUAT;
	if (StringHelp::compare("color", property_type, true) == 0)
		return PropertySerializer::TYPE_COLOR;
	if (StringHelp::compare("texture", property_type, true) == 0)
		return PropertySerializer::TYPE_TEXTURE_NAME;
	throw Exception(string_format("PropertySerializer::get_property_type - Unknown property type %1", property_type));
}

PropertySerializer::PropertyType PropertySerializer::get_property_type(std::shared_ptr<IProperty> property)
{
	return get_property_type(property.get());
}

PropertySerializer::PropertyType PropertySerializer::get_property_type(IProperty *property)
{
	if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<unsigned int>())
	{
		return TYPE_UNSIGNED_INT;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<int>())
	{
		return TYPE_INT;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<float>())
	{
		return TYPE_FLOAT;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<bool>())
	{
		return TYPE_BOOL;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<double>())
	{
		return TYPE_DOUBLE;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<std::string>())
	{
		return TYPE_STRING;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::vec2>())
	{
		return TYPE_VEC2;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::vec3>())
	{
		return TYPE_VEC3;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::vec4>())
	{
		return TYPE_VEC4;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::quat>())
	{
		return TYPE_QUAT;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<Color>())
	{
		return TYPE_COLOR;
	}
	else 
	{
		throw Exception("PropertySerializer::get_property_type: Unknown type " + property->get_name());
	}
}

std::string PropertySerializer::property_value_to_string(IProperty *property)
{
	if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<unsigned int>())
	{
		const unsigned int &value = static_cast<Property<unsigned int> *>(property)->get();
		return StringHelp::uint_to_text(value);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<int>())
	{
		const int &value = static_cast<Property<int> *>(property)->get();
		return StringHelp::int_to_text(value);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<float>())
	{
		const float &value = static_cast<Property<float> *>(property)->get();
		return StringHelp::float_to_text(value);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<bool>())
	{
		const bool &value = static_cast<Property<bool> *>(property)->get();
		return StringHelp::bool_to_text(value);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<double>())
	{
		const double &value = static_cast<Property<double> *>(property)->get();
		return StringHelp::double_to_text(value);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<char>())
	{
		const char &value = static_cast<Property<char> *>(property)->get();
		return StringHelp::int_to_text((int)value);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<std::string>())
	{
		const std::string &value = static_cast<Property<std::string> *>(property)->get();
		return value;
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::vec2>())
	{
		const glm::vec2 &value = static_cast<Property<glm::vec2> *>(property)->get();
		return string_format("%1 %2", value.x, value.y);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::vec3>())
	{
		const glm::vec3 &value = static_cast<Property<glm::vec3> *>(property)->get();
		return string_format("%1 %2 %3", value.x, value.y, value.z);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::vec4>())
	{
		const glm::vec4 &value = static_cast<Property<glm::vec4> *>(property)->get();
		return string_format("%1 %2 %3 %4", value.x, value.y, value.z, value.w);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<glm::quat>())
	{
		const glm::quat &value = static_cast<Property<glm::quat> *>(property)->get();
		return string_format("%1 %2 %3 %4", value.x, value.y, value.z, value.w);
	}
	else if (property->get_runtime_type_id() == IProperty::get_runtime_type_id<Color>())
	{
		const Color &value = static_cast<Property<Color> *>(property)->get();
		return value.get_rgba8_hexstring();
	}
	else 
	{
		throw Exception("PropertySerializer::property_value_to_string: Unknown type " + property->get_name());
	}
}

std::string PropertySerializer::property_value_to_string(std::shared_ptr<IProperty> property)
{
	return property_value_to_string(property.get());
}

void PropertySerializer::from_string(const std::string &value, unsigned int &result)
{
	result = StringHelp::text_to_uint(value);
}

void PropertySerializer::from_string(const std::string &value, int &result)
{
	result = StringHelp::text_to_int(value);
}

void PropertySerializer::from_string(const std::string &value, float &result)
{
	result = StringHelp::text_to_float(value);
}

void PropertySerializer::from_string(const std::string &value, double &result)
{
	result = StringHelp::text_to_double(value);
}

void PropertySerializer::from_string(const std::string &value, bool &result)
{
	result = StringHelp::text_to_bool(value);
}

void PropertySerializer::from_string(const std::string &value, char &result)
{
	result = value.c_str()[0];
}

void PropertySerializer::from_string(const std::string &value, std::string &result)
{
	result = value;
}

void PropertySerializer::from_string(const std::string &value, glm::vec2 &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if(values.size() != 2)
		throw Exception("vec2 from_string failed");

	float x = StringHelp::text_to_float(values[0]);
	float y = StringHelp::text_to_float(values[1]);

	result = glm::vec2(x, y);
}

void PropertySerializer::from_string(const std::string &value, glm::vec3 &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if(values.size() != 3)
		throw Exception("vec3 from_string failed");

	float x = StringHelp::text_to_float(values[0]);
	float y = StringHelp::text_to_float(values[1]);
	float z = StringHelp::text_to_float(values[2]);

	result = glm::vec3(x, y, z);
}

void PropertySerializer::from_string(const std::string &value, glm::vec4 &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if(values.size() != 4)
		throw Exception("vec4 from_string failed");

	float x = StringHelp::text_to_float(values[0]);
	float y = StringHelp::text_to_float(values[1]);
	float z = StringHelp::text_to_float(values[2]);
	float w = StringHelp::text_to_float(values[3]);

	result = glm::vec4(x, y, z, w);
}

void PropertySerializer::from_string(const std::string &value, glm::quat &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if (values.size() != 4)
		throw Exception("quat from_string failed");

	float x = StringHelp::text_to_float(values[0]);
	float y = StringHelp::text_to_float(values[1]);
	float z = StringHelp::text_to_float(values[2]);
	float w = StringHelp::text_to_float(values[3]);

	result = glm::quat(x, y, z, w);
}

void PropertySerializer::from_string(const std::string &value, Color &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if (values.size() == 4)
	{
		int r = StringHelp::text_to_int(values[0]);
		int g = StringHelp::text_to_int(values[1]);
		int b = StringHelp::text_to_int(values[2]);
		int a = StringHelp::text_to_int(values[3]);

		result = Color(r, g, b, a);
	}
	else if (values.size() == 3)
	{
		int r = StringHelp::text_to_int(values[0]);
		int g = StringHelp::text_to_int(values[1]);
		int b = StringHelp::text_to_int(values[2]);

		result = Color(r, g, b);
	}
	else if (values.size() == 1)
	{
		result = Color(value);
	}
	else
	{
		throw Exception("Color from_string failed");
	}
}
