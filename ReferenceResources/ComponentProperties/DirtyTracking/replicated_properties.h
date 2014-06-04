
#pragma once

class ReplicatedProperties
{
public:
	enum ReplicationMode
	{
		REPLICATE_ON_CREATION = 1,
		REPLICATE_ON_CHANGE = 2
	};

public:
	ReplicatedProperties();

	void replicate_property(Totem::Property<unsigned int> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<int> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<float> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<double> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<bool> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<char> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<std::string> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<clan::Vec2i> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<clan::Vec3f> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);
	void replicate_property(Totem::Property<clan::Colorf> &property, ReplicationMode mode = REPLICATE_ON_CHANGE);

	std::vector<Totem::IProperty *> get_tracked_properties();
	std::vector<Totem::IProperty *> get_dirty_tracked_properties();
	void clear_dirty_tracked_properties();

private:
	void on_uint_value_changed(const unsigned int &old_value, const unsigned int &new_value, Totem::IProperty *property);
	void on_int_value_changed(const int &old_value, const int &new_value, Totem::IProperty *property);
	void on_float_value_changed(const float &old_value, const float &new_value, Totem::IProperty *property);
	void on_double_value_changed(const double &old_value, const double &new_value, Totem::IProperty *property);
	void on_bool_value_changed(const bool &old_value, const bool &new_value, Totem::IProperty *property);
	void on_char_value_changed(const char &old_value, const char &new_value, Totem::IProperty *property);
	void on_cl_string_value_changed(const std::string &old_value, const std::string &new_value, Totem::IProperty *property);
	void on_cl_vec2i_value_changed(const clan::Vec2i &old_value, const clan::Vec2i &new_value, Totem::IProperty *property);
	void on_cl_vec3f_value_changed(const clan::Vec3f &old_value, const clan::Vec3f &new_value, Totem::IProperty *property);
	void on_cl_colorf_value_changed(const clan::Colorf &old_value, const clan::Colorf &new_value, Totem::IProperty *property);

	//This function simply reduce duplication of the same stuff between all these replicate property implementations
	Totem::IProperty *replicate_property(Totem::IProperty *property);

	std::map<Totem::IProperty *, bool /*dirty*/> properties;

	clan::CallbackContainer slots;
};
