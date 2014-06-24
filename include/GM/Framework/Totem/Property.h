#pragma once
#include <ClanLib/core.h>

#if defined(_MSC_VER) // for visual studio
#    pragma warning(push)
#    pragma warning(disable : 4481)
#endif

#include "IProperty.h"

 
 

namespace GM {
namespace Framework {

template<class PropertyType>
class PropertyData
{
public:
	PropertyType value;
	std::string name;
	bool dirty;
	clan::Signal<const PropertyType &, const PropertyType &> value_changed;
};

template<class PropertyType>
class Property : public IProperty
{
public:
	Property();
	Property(const Property &copy);
	Property(const std::string &name);

	void set(const PropertyType& value, bool invoke_value_changed = true);
	const PropertyType &get() const;
	PropertyType &get();
	const std::string &get_name() const override;
	bool is_null() const override;
	bool is_dirty() const override;
	void clear_dirty() override;
	unsigned int get_runtime_type_id() const override;

	clan::Signal<const PropertyType &, const PropertyType &> &value_changed();

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

} // namespace Framework
} // namespace Gm

#if defined(_MSC_VER)
#    pragma warning(pop)
#endif
