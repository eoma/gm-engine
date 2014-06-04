#include "precomp.h"
#include "property_serializer.h"

using namespace Totem;
using namespace clan;

void PropertySerializer::create_and_add_property(Totem::PropertyContainer<> &container, int type_id, const std::string &name, const std::string &value)
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
		case PropertySerializer::TYPE_CL_VEC2I:
		{
			Vec2i result;
			from_string(value, result);
			Property<Vec2i> property = container.add<Vec2i>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_CL_VEC3F:
		{
			Vec3f result;
			from_string(value, result);
			Property<Vec3f> property = container.add<Vec3f>(name, result);
			property.set(result);
			break;
		}
		case PropertySerializer::TYPE_CL_COLORF:
		{
			Colorf result;
			from_string(value, result);
			Property<Colorf> property = container.add<Colorf>(name, result);
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
	if(StringHelp::compare("vec2i", property_type, true) == 0)
		return PropertySerializer::TYPE_CL_VEC2I;
	if(StringHelp::compare("vec3f", property_type, true) == 0)
		return PropertySerializer::TYPE_CL_VEC3F;
	if(StringHelp::compare("colorf", property_type, true) == 0)
		return PropertySerializer::TYPE_CL_COLORF;
	throw Exception(string_format("PropertySerializer::get_property_type - Unknown property type %1", property_type));
}

PropertySerializer::PropertyType PropertySerializer::get_property_type(std::shared_ptr<Totem::IProperty> property)
{
	return get_property_type(property.get());
}

PropertySerializer::PropertyType PropertySerializer::get_property_type(Totem::IProperty *property)
{
	if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<unsigned int>())
	{
		return TYPE_UNSIGNED_INT;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<int>())
	{
		return TYPE_INT;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<float>())
	{
		return TYPE_FLOAT;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<bool>())
	{
		return TYPE_BOOL;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<double>())
	{
		return TYPE_DOUBLE;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<std::string>())
	{
		return TYPE_STRING;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<Vec2i>())
	{
		return TYPE_CL_VEC2I;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<Vec3f>())
	{
		return TYPE_CL_VEC3F;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<Colorf>())
	{
		return TYPE_CL_COLORF;
	}
	else 
	{
		throw Exception("PropertySerializer::get_property_type: Unknown type " + property->getName());
	}
}

std::string PropertySerializer::property_value_to_string(Totem::IProperty *property)
{
	if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<unsigned int>())
	{
		const unsigned int &value = static_cast<Totem::Property<unsigned int> *>(property)->get();
		return StringHelp::uint_to_text(value);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<int>())
	{
		const int &value = static_cast<Totem::Property<int> *>(property)->get();
		return StringHelp::int_to_text(value);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<float>())
	{
		const float &value = static_cast<Totem::Property<float> *>(property)->get();
		return StringHelp::float_to_text(value);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<bool>())
	{
		const bool &value = static_cast<Totem::Property<bool> *>(property)->get();
		return StringHelp::bool_to_text(value);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<double>())
	{
		const double &value = static_cast<Totem::Property<double> *>(property)->get();
		return StringHelp::double_to_text(value);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<char>())
	{
		const char &value = static_cast<Totem::Property<char> *>(property)->get();
		return StringHelp::int_to_text((int)value);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<std::string>())
	{
		const std::string &value = static_cast<Totem::Property<std::string> *>(property)->get();
		return value;
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<Vec2i>())
	{
		const Vec2i &value = static_cast<Totem::Property<Vec2i> *>(property)->get();
		return string_format("%1 %2", value.x, value.y);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<Vec3f>())
	{
		const Vec3f &value = static_cast<Totem::Property<Vec3f> *>(property)->get();
		return string_format("%1 %2 %3", value.x, value.y, value.z);
	}
	else if(property->getRuntimeTypeId() == Totem::IProperty::getRuntimeTypeId<Colorf>())
	{
		const Colorf &value = static_cast<Totem::Property<Colorf> *>(property)->get();
		return string_format("%1 %2 %3 %4", value.r, value.g, value.b, value.a);
	}
	else 
	{
		throw Exception("PropertySerializer::property_value_to_string: Unknown type " + property->getName());
	}
}

std::string PropertySerializer::property_value_to_string(std::shared_ptr<Totem::IProperty> property)
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

void PropertySerializer::from_string(const std::string &value, Vec2i &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if(values.size() != 2)
		throw Exception("Vec2i from_string failed");

	int x = StringHelp::text_to_int(values[0]);
	int y = StringHelp::text_to_int(values[1]);

	result = Vec2i(x, y);
}

void PropertySerializer::from_string(const std::string &value, Vec3f &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if(values.size() != 3)
		throw Exception("Vec3f from_string failed");

	float x = StringHelp::text_to_float(values[0]);
	float y = StringHelp::text_to_float(values[1]);
	float z = StringHelp::text_to_float(values[2]);

	result = Vec3f(x, y, z);
}

void PropertySerializer::from_string(const std::string &value, Colorf &result)
{
	std::vector<std::string> values = StringHelp::split_text(value, " ");
	if(values.size() != 4)
		throw Exception("Colorf from_string failed");

	float r = StringHelp::text_to_float(values[0]);
	float g = StringHelp::text_to_float(values[1]);
	float b = StringHelp::text_to_float(values[2]);
	float a = StringHelp::text_to_float(values[3]);

	result = Colorf(r, g, b, a);
}
