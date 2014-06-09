#pragma once
#include <ClanLib/core.h>

#include "Property.h"

 
#include <unordered_map>
#include <vector>
 

namespace Totem
{

class PropertyFactory
{
public:
	template<class PropertyType> static std::shared_ptr<Totem::Property<PropertyType>> createProperty(const CL_String &name);
};

template<class UserData = void*>
class PropertyContainer
{
public:
	PropertyContainer();
	virtual ~PropertyContainer();

	bool hasProperty(const CL_String& name);

	template<class T> Property<T> add(const CL_String& name, const T &defaultValue);
	template<class T> Property<T> add(const CL_String& name, const T &defaultValue, const UserData &userData);
	void add(std::shared_ptr<IProperty> property);

	template<class T> Property<T> get(const CL_String& name);
	std::unordered_map<CL_String, std::shared_ptr<IProperty>> &getProperties();

	void removeProperty(const CL_String& name, bool postponeDelete = false);
	void removeProperty(const CL_String& name, const UserData &userData, bool postponeDelete = false);
	void removeAllProperties();
	void removeAllProperties(const UserData &userData);

	void clearDeletedProperties();
	void clearDirtyProperties();

	void updateProperties();

	PropertyContainer &operator= (const PropertyContainer &rhs);

	CL_Signal_v<std::shared_ptr<IProperty>> &propertyAdded();
	CL_Signal_v<std::shared_ptr<IProperty>, const UserData&> &propertyWithUserDataAdded();
	CL_Signal_v<std::shared_ptr<IProperty>> &propertyRemoved();
	CL_Signal_v<std::shared_ptr<IProperty>, const UserData&> &propertyWithUserDataRemoved();

protected:
	std::unordered_map<CL_String, std::shared_ptr<IProperty>> properties;
	std::vector<std::shared_ptr<IProperty>> deletedProperties;

	CL_Signal_v<std::shared_ptr<IProperty>> sign_PropertyAdded;
	CL_Signal_v<std::shared_ptr<IProperty>, const UserData&> sign_PropertyWithUserDataAdded;
	CL_Signal_v<std::shared_ptr<IProperty>> sign_PropertyRemoved;
	CL_Signal_v<std::shared_ptr<IProperty>, const UserData&> sign_PropertyWithUserDataRemoved;
};

#include "PropertyContainer.inl"

} //namespace Totem 
