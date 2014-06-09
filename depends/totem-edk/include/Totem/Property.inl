
template<class PropertyType>
Property<PropertyType>::Property() {}

template<class PropertyType>
Property<PropertyType>::Property(const Property &copy)
	: data(copy.data)
{
}

template<class PropertyType>
Property<PropertyType>::Property(const std::string &name)
	: data(std::make_shared<PropertyData<PropertyType>>())
{
	data->name = name;
	data->dirty = false; 
}

template<class PropertyType>
inline void Property<PropertyType>::set(const PropertyType& value, bool invokeValueChanged) 
{ 
	if(data->value != value)
	{
		PropertyType oldValue = data->value;
		data->value = value; 
		data->dirty = true; 

		if(invokeValueChanged)
			data->valueChanged.invoke(oldValue, value);
	}
}

template<class PropertyType>
inline unsigned int Property<PropertyType>::getRuntimeTypeId() const { return IProperty::getRuntimeTypeId<PropertyType>(); }

template<class PropertyType>
inline const PropertyType &Property<PropertyType>::get() const { return data->value; }

template<class PropertyType>
inline PropertyType &Property<PropertyType>::get() { return data->value; }

template<class PropertyType>
inline const std::string &Property<PropertyType>::getName() const { return data->name; }

template<class PropertyType>
inline bool Property<PropertyType>::isNull() const { return data == nullptr; }

template<class PropertyType>
inline bool Property<PropertyType>::isDirty() const { return data->dirty; }

template<class PropertyType>
inline void Property<PropertyType>::clearDirty() { data->dirty = false; }

template<class PropertyType>
inline sigslot::signal<const PropertyType &, const PropertyType &> &Property<PropertyType>::valueChanged() { return data->valueChanged; }

/// Instead of property.get() this operator exist for convenience.
template<class PropertyType>
inline Property<PropertyType>::operator const PropertyType &() const { return data->value; }
/// Instead of property.get() this operator exist for convenience.
template<class PropertyType>
inline Property<PropertyType>::operator PropertyType &() { return data->value; }

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator =(const Property<PropertyType> &rhs)
{
	data = rhs.data;
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator =(const PropertyType &rhs)
{
	set(rhs);
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator +=(const Property<PropertyType> &rhs)
{
	set(data->value + rhs.data->value);
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator +=(const PropertyType &rhs)
{
	set(data->value + rhs);
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator -=(const Property<PropertyType> &rhs)
{
	set(data->value - rhs.data->value);
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator -=(const PropertyType &rhs)
{
	set(data->value - rhs);
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator *=(const Property<PropertyType> &rhs)
{
	set(data->value * rhs.data->value);
	return *this;
}

template<class PropertyType>
inline Property<PropertyType> Property<PropertyType>::operator *=(const PropertyType &rhs)
{
	set(data->value * rhs);
	return *this;
}

template<class PropertyType>
inline bool Property<PropertyType>::operator ==(const Property<PropertyType> &rhs)
{
	return data == rhs.data;
}

template<class PropertyType>
inline bool Property<PropertyType>::operator ==(const PropertyType &rhs)
{
	return (data->value == rhs);
}

template<class PropertyType>
inline bool Property<PropertyType>::operator !=(const Property<PropertyType> &rhs)
{
	return data != rhs.data;
}

template<class PropertyType>
inline bool Property<PropertyType>::operator !=(const PropertyType &rhs)
{
	return (data->value != rhs);
}

template<class PropertyType>
inline bool Property<PropertyType>::operator >(const Property<PropertyType> &rhs)
{
	return (data->value > rhs.data->value);
}

template<class PropertyType>
inline bool Property<PropertyType>::operator >(const PropertyType &rhs)
{
	return (data->value > rhs);
}

template<class PropertyType>
inline bool Property<PropertyType>::operator <(const Property<PropertyType> &rhs)
{
	return (data->value < rhs.data->value);
}

template<class PropertyType>
inline bool Property<PropertyType>::operator <(const PropertyType &rhs)
{
	return (data->value < rhs);
}

