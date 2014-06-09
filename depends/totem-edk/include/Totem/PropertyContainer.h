#pragma once

#include "Property.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Totem
{

class PropertyFactory
{
public:
	template<class PropertyType> static std::shared_ptr<Totem::Property<PropertyType>> createProperty(const std::string &name);
};

template<class UserData = void*>
class PropertyContainer
{
public:
	PropertyContainer();
	virtual ~PropertyContainer();

	bool hasProperty(const std::string& name);

	template<class T> Property<T> add(const std::string& name, const T &defaultValue);
	template<class T> Property<T> add(const std::string& name, const T &defaultValue, const UserData &userData);
	void add(std::shared_ptr<IProperty> property);

	template<class T> Property<T> get(const std::string& name);
	std::unordered_map<std::string, std::shared_ptr<IProperty>> &getProperties();

	void removeProperty(const std::string& name, bool postponeDelete = false);
	void removeProperty(const std::string& name, const UserData &userData, bool postponeDelete = false);
	void removeAllProperties();
	void removeAllProperties(const UserData &userData);

	void clearDeletedProperties();
	void clearDirtyProperties();

	void updateProperties();

	PropertyContainer &operator= (const PropertyContainer &rhs);

	sigslot::signal<std::shared_ptr<IProperty>> &propertyAdded();
	sigslot::signal<std::shared_ptr<IProperty>, const UserData&> &propertyWithUserDataAdded();
	sigslot::signal<std::shared_ptr<IProperty>> &propertyRemoved();
	sigslot::signal<std::shared_ptr<IProperty>, const UserData&> &propertyWithUserDataRemoved();

protected:
	std::unordered_map<std::string, std::shared_ptr<IProperty>> properties;
	std::vector<std::shared_ptr<IProperty>> deletedProperties;

	sigslot::signal<std::shared_ptr<IProperty>> sign_PropertyAdded;
	sigslot::signal<std::shared_ptr<IProperty>, const UserData&> sign_PropertyWithUserDataAdded;
	sigslot::signal<std::shared_ptr<IProperty>> sign_PropertyRemoved;
	sigslot::signal<std::shared_ptr<IProperty>, const UserData&> sign_PropertyWithUserDataRemoved;
};

#include "PropertyContainer.inl"

} //namespace Totem 
