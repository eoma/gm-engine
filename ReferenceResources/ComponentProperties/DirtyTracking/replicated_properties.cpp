
#include "precomp.h"
#include "replicated_properties.h"

using namespace Totem;
using namespace clan;

ReplicatedProperties::ReplicatedProperties()
{
}

/* TODO: probably need this constructor to support component based properties
ReplicatedProperties::ReplicatedProperties(const std::string &component_name)
{
}
*/

Totem::IProperty *ReplicatedProperties::replicate_property(Totem::IProperty *property)
{
	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		if(it->first->getName() == property->getName())
		{
			return nullptr;
		}
	}

	properties[property] = false;
	return property;
}

void ReplicatedProperties::replicate_property(Totem::Property<unsigned int> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if (mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const unsigned int &, const unsigned int &)>(
			this, &ReplicatedProperties::on_uint_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<int> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const int &, const int &)>(
			this, &ReplicatedProperties::on_int_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<float> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const float &, const float &)>(
			this, &ReplicatedProperties::on_float_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<double> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const double &, const double &)>(
			this, &ReplicatedProperties::on_double_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<bool> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const bool &, const bool &)>(
			this, &ReplicatedProperties::on_bool_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<char> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const char &, const char &)>(
			this, &ReplicatedProperties::on_char_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<std::string> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const std::string &, const std::string &)>(
			this, &ReplicatedProperties::on_cl_string_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<Vec2i> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const clan::Vec2i &, const clan::Vec2i &)>(
			this, &ReplicatedProperties::on_cl_vec2i_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<Vec3f> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const clan::Vec3f &, const clan::Vec3f &)>(
			this, &ReplicatedProperties::on_cl_vec3f_value_changed, iproperty));
}

void ReplicatedProperties::replicate_property(Totem::Property<Colorf> &property, ReplicationMode mode)
{
	IProperty *iproperty = replicate_property(&property);
	if(iproperty == nullptr)
		return;

	if(mode == REPLICATE_ON_CHANGE)
		slots.connect(property.valueChanged(), 
			clan::Callback<void(const clan::Colorf &, const clan::Colorf &)>(
			this, &ReplicatedProperties::on_cl_colorf_value_changed, iproperty));
}

std::vector<Totem::IProperty *> ReplicatedProperties::get_tracked_properties()
{
	std::vector<IProperty *> results;

	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		results.push_back(it->first);
	}

	return results;
}

std::vector<IProperty *> ReplicatedProperties::get_dirty_tracked_properties()
{
	std::vector<IProperty *> results;

	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		if(it->second == true) // Is property dirty?
		{
			results.push_back(it->first);
		}
	}

	return results;
}

void ReplicatedProperties::clear_dirty_tracked_properties()
{
	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		it->second = false;
	}
}

void ReplicatedProperties::on_uint_value_changed(const unsigned int &/*old_value*/, const unsigned int &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_int_value_changed(const int &/*old_value*/, const int &/*new_value*/, IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_float_value_changed(const float &/*old_value*/, const float &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_double_value_changed(const double &/*old_value*/, const double &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_bool_value_changed(const bool &/*old_value*/, const bool &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_cl_string_value_changed(const std::string &/*old_value*/, const std::string &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_cl_vec2i_value_changed(const Vec2i &/*old_value*/, const Vec2i &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_cl_vec3f_value_changed(const Vec3f &/*old_value*/, const Vec3f &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_cl_colorf_value_changed(const Colorf &/*old_value*/, const Colorf &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}

void ReplicatedProperties::on_char_value_changed(const char &/*old_value*/, const char &/*new_value*/, Totem::IProperty *property)
{
	properties[property] = true;
}
