#pragma once

#include "PropertyList.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Totem
{

class PropertyListFactory
{
public:
	template<class PropertyType> static std::shared_ptr<Totem::PropertyList<PropertyType>> createPropertyList(const std::string &name);
};

template<class UserData = void*>
class PropertyListContainer
{
public:
	PropertyListContainer();
	virtual ~PropertyListContainer();

	bool hasPropertyList(const std::string& name);

	template<class T> PropertyList<T> addList(const std::string& name);
	template<class T> PropertyList<T> addList(const std::string& name, const UserData &userData);
	void addList(std::shared_ptr<IPropertyList> property);

	template<class T> PropertyList<T> getList(const std::string& name);
	std::shared_ptr<IPropertyList> getListInterface(const std::string& name);
	std::unordered_map<std::string, std::shared_ptr<IPropertyList>> &getPropertyLists();

	void removePropertyList(const std::string& name, bool postponeDelete = false);
	void removePropertyList(const std::string& name, const UserData &userData, bool postponeDelete = false);
	void removeAllPropertyLists();

	void clearDeletedPropertyLists();
	void clearDirtyPropertyLists();

	void updatePropertyLists();

	PropertyListContainer &operator= (const PropertyListContainer &rhs);

	sigslot::signal<std::shared_ptr<IPropertyList>> &propertyListAdded();
	sigslot::signal<std::shared_ptr<IPropertyList>, const UserData&> &propertyListWithUserDataAdded();
	sigslot::signal<std::shared_ptr<IPropertyList>> &propertyListRemoved();
	sigslot::signal<std::shared_ptr<IPropertyList>, const UserData&> &propertyListWithUserDataRemoved();

protected:
	std::unordered_map<std::string, std::shared_ptr<IPropertyList>> propertyLists;
	std::vector<std::shared_ptr<IPropertyList>> deletedPropertyLists;

	sigslot::signal<std::shared_ptr<IPropertyList>> sign_PropertyListAdded;
	sigslot::signal<std::shared_ptr<IPropertyList>, const UserData&> sign_PropertyListWithUserDataAdded;
	sigslot::signal<std::shared_ptr<IPropertyList>> sign_PropertyListRemoved;
	sigslot::signal<std::shared_ptr<IPropertyList>, const UserData&> sign_PropertyListWithUserDataRemoved;
};

#include "PropertyListContainer.inl"

} //namespace Totem 
