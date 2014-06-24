#pragma once
#include <ClanLib/core.h>

#include "Property.h"

 
#include <unordered_map>
#include <vector>
 

namespace GM {
namespace Framework {

class PropertyFactory
{
public:
	template<class PropertyType> static std::shared_ptr<Property<PropertyType>> create_property(const std::string &name);
};

template<class UserData = void*>
class PropertyContainer
{
public:
	PropertyContainer();
	virtual ~PropertyContainer();

	bool has_property(const std::string& name);

	template<class T> Property<T> add(const std::string& name, const T &default_value);
	template<class T> Property<T> add(const std::string& name, const T &default_value, const UserData &user_data);
	void add(std::shared_ptr<IProperty> property);

	template<class T> Property<T> get(const std::string& name);
	std::unordered_map<std::string, std::shared_ptr<IProperty>> &get_properties();

	void remove_property(const std::string& name, bool postpone_delete = false);
	void remove_property(const std::string& name, const UserData &user_data, bool postpone_delete = false);
	void remove_all_properties();
	void remove_all_properties(const UserData &user_data);

	void clear_deleted_properties();
	void clear_dirty_properties();

	void update_properties();

	PropertyContainer &operator= (const PropertyContainer &rhs);

	clan::Signal<std::shared_ptr<IProperty>> &property_added();
	clan::Signal<std::shared_ptr<IProperty>, const UserData&> &property_with_user_data_added();
	clan::Signal<std::shared_ptr<IProperty>> &property_removed();
	clan::Signal<std::shared_ptr<IProperty>, const UserData&> &property_with_user_data_removed();

protected:
	std::unordered_map<std::string, std::shared_ptr<IProperty>> properties;
	std::vector<std::shared_ptr<IProperty>> deleted_properties;

	clan::Signal<std::shared_ptr<IProperty>> sign_PropertyAdded;
	clan::Signal<std::shared_ptr<IProperty>, const UserData&> sign_PropertyWithUserDataAdded;
	clan::Signal<std::shared_ptr<IProperty>> sign_PropertyRemoved;
	clan::Signal<std::shared_ptr<IProperty>, const UserData&> sign_PropertyWithUserDataRemoved;
};

#include "PropertyContainer.inl"

} // namespace Framework
} // namespace GM
