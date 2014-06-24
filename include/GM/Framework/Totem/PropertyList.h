#pragma once
#include <ClanLib/core.h>

#if defined(_MSC_VER) // for visual studio
#     pragma warning(push)
#     pragma warning(disable : 4481)
#endif

#include "IPropertyList.h"

 
 
#include <vector>

namespace GM {
namespace Framework {

template<class PropertyType>
class PropertyListData
{
public:
	std::vector<PropertyType> value;
	std::string name;
	bool dirty;
	clan::Signal<unsigned int, const PropertyType &, const PropertyType &> value_changed;
	clan::Signal<unsigned int, const PropertyType &> value_added;
	clan::Signal<unsigned int, const PropertyType &> value_erased;
	clan::Signal<> values_cleared;
	clan::Signal<unsigned int, unsigned int> list_resized;
};

template<class PropertyType>
class PropertyListIndexValue
{
public:
	PropertyListIndexValue(std::shared_ptr<PropertyListData<PropertyType>> data, unsigned int index);

	const PropertyType &get() const;
	PropertyType &get();
	void set(const PropertyType &rhs, bool invoke_value_changed = true);

	void operator= (const PropertyType &rhs);
	PropertyListIndexValue<PropertyType> &operator= (const PropertyListIndexValue<PropertyType> &rhs);
	operator const PropertyType &() const { return get(); }
	PropertyListIndexValue<PropertyType> &operator+= (const PropertyListIndexValue<PropertyType>& rhs);
	PropertyListIndexValue<PropertyType> &operator+= (const PropertyType& rhs);
	PropertyListIndexValue<PropertyType> &operator-= (const PropertyListIndexValue<PropertyType>& rhs);
	PropertyListIndexValue<PropertyType> &operator-= (const PropertyType& rhs);
	PropertyListIndexValue<PropertyType> &operator*= (const PropertyListIndexValue<PropertyType>& rhs);
	PropertyListIndexValue<PropertyType> &operator*= (const PropertyType& rhs);
	PropertyListIndexValue<PropertyType> &operator/= (const PropertyListIndexValue<PropertyType>& rhs);
	PropertyListIndexValue<PropertyType> &operator/= (const PropertyType& rhs);

private:
	std::shared_ptr<PropertyListData<PropertyType>> data;
	unsigned int index;
};

template<class PropertyType>
class PropertyList : public IPropertyList
{
public:
	PropertyList();
	PropertyList(const PropertyList &copy);
	PropertyList(const std::string &name);

	void push_back(const PropertyType& value, bool invoke_value_added = true);
	void erase(unsigned int index, bool invoke_value_erased = true);
	void clear(bool invoke_values_cleared = true);
	unsigned int size() const;
	bool empty() const;
	void resize(unsigned int size, bool invoke_list_resized = true);
	void resize(unsigned int size, const PropertyType &value, bool invoke_list_resized = true);
	PropertyListIndexValue<PropertyType> at(unsigned int index);

	unsigned int get_runtime_type_id() const override;

	const std::vector<PropertyType> &get() const;
	std::vector<PropertyType> &get();
	const std::string &get_name() const override;
	bool is_null() const override;
	bool is_dirty() const override;
	void clear_dirty() override;

	clan::Signal<unsigned int, const PropertyType &, const PropertyType &> &value_changed();
	clan::Signal<unsigned int, const PropertyType &> &value_added();
	clan::Signal<unsigned int, const PropertyType &> &value_erased();
	clan::Signal<> &values_cleared();
	clan::Signal<unsigned int, unsigned int> &list_resized();

	PropertyList<PropertyType> operator= (const PropertyList<PropertyType>& rhs);
	PropertyListIndexValue<PropertyType> operator[] (const unsigned int& index);
	operator const std::vector<PropertyType> &() const { return data->value; }

private:
	std::shared_ptr<PropertyListData<PropertyType>> data;
};

#include "PropertyList.inl"

} // namespace Framework
} // namespace GM

#if defined(_MSC_VER)
#    pragma warning(pop)
#endif
