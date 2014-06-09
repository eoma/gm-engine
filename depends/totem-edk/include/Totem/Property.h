#pragma once
#pragma warning(push)
#pragma warning(disable : 4481)

#include "IProperty.h"

#include <sigslot.h>
#include <memory>

namespace Totem
{

template<class PropertyType>
class PropertyData
{
public:
	PropertyType value;
	std::string name;
	bool dirty;
	sigslot::signal<const PropertyType &, const PropertyType &> valueChanged;
};

template<class PropertyType>
class Property : public IProperty
{
public:
	Property();
	Property(const Property &copy);
	Property(const std::string &name);

	void set(const PropertyType& value, bool invokeValueChanged = true);
	const PropertyType &get() const;
	PropertyType &get();
	const std::string &getName() const override;
	bool isNull() const override;
	bool isDirty() const override;
	void clearDirty() override;
	unsigned int getRuntimeTypeId() const override;

	sigslot::signal<const PropertyType &, const PropertyType &> &valueChanged();

	Property<PropertyType> operator= (const Property<PropertyType>& rhs);
	Property<PropertyType> operator= (const PropertyType& rhs);
	Property<PropertyType> operator+= (const Property<PropertyType>& rhs);
	Property<PropertyType> operator+= (const PropertyType& rhs);
	Property<PropertyType> operator-= (const Property<PropertyType>& rhs);
	Property<PropertyType> operator-= (const PropertyType& rhs);
	Property<PropertyType> operator*= (const Property<PropertyType>& rhs);
	Property<PropertyType> operator*= (const PropertyType& rhs);
	bool operator== (const Property<PropertyType>& rhs);
	bool operator== (const PropertyType& rhs);
	bool operator!= (const Property<PropertyType>& rhs);
	bool operator!= (const PropertyType& rhs);
	bool operator> (const Property<PropertyType>& rhs);
	bool operator> (const PropertyType& rhs);
	bool operator< (const Property<PropertyType>& rhs);
	bool operator< (const PropertyType& rhs);
	operator const PropertyType &() const;
	operator PropertyType &();

private:
	std::shared_ptr<PropertyData<PropertyType>> data;
};

#include "Property.inl"

} //namespace Totem

#pragma warning(pop)
