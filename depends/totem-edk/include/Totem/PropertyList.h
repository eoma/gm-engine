#pragma once
#pragma warning(push)
#pragma warning(disable : 4481)

#include "IPropertyList.h"

#include <sigslot.h>
#include <memory>
#include <vector>

namespace Totem
{

template<class PropertyType>
class PropertyListData
{
public:
	std::vector<PropertyType> value;
	std::string name;
	bool dirty;
	sigslot::signal<unsigned int, const PropertyType &, const PropertyType &> valueChanged;
	sigslot::signal<unsigned int, const PropertyType &> valueAdded;
	sigslot::signal<unsigned int, const PropertyType &> valueErased;
	sigslot::signal<> valuesCleared;
	sigslot::signal<unsigned int, unsigned int> listResized;
};

template<class PropertyType>
class PropertyListIndexValue
{
public:
	PropertyListIndexValue(std::shared_ptr<PropertyListData<PropertyType>> data, unsigned int index);

	const PropertyType &get() const;
	PropertyType &get();
	void set(const PropertyType &rhs, bool invokeValueChanged = true);

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

	void push_back(const PropertyType& value, bool invokeValueAdded = true);
	void erase(unsigned int index, bool invokeValueErased = true);
	void clear(bool invokeValuesCleared = true);
	unsigned int size() const;
	bool empty() const;
	void resize(unsigned int size, bool invokeListResized = true);
	void resize(unsigned int size, const PropertyType &value, bool invokeListResized = true);
	PropertyListIndexValue<PropertyType> at(unsigned int index);

	unsigned int getRuntimeTypeId() const override;

	const std::vector<PropertyType> &get() const;
	std::vector<PropertyType> &get();
	const std::string &getName() const override;
	bool isNull() const override;
	bool isDirty() const override;
	void clearDirty() override;

	sigslot::signal<unsigned int, const PropertyType &, const PropertyType &> &valueChanged();
	sigslot::signal<unsigned int, const PropertyType &> &valueAdded();
	sigslot::signal<unsigned int, const PropertyType &> &valueErased();
	sigslot::signal<> &valuesCleared();
	sigslot::signal<unsigned int, unsigned int> &listResized();

	PropertyList<PropertyType> operator= (const PropertyList<PropertyType>& rhs);
	PropertyListIndexValue<PropertyType> operator[] (const unsigned int& index);
	operator const std::vector<PropertyType> &() const { return data->value; }

private:
	std::shared_ptr<PropertyListData<PropertyType>> data;
};

#include "PropertyList.inl"

} //namespace Totem

#pragma warning(pop)
