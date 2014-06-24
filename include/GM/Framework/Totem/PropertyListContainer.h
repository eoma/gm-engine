#pragma once
#include <ClanLib/core.h>

#include "PropertyList.h"

 
#include <unordered_map>
#include <vector>
 

namespace GM {
namespace Framework {

class PropertyListFactory
{
public:
	template<class PropertyType> static std::shared_ptr<GM::Framework::PropertyList<PropertyType>> create_property_list(const std::string &name);
};

template<class UserData = void*>
class PropertyListContainer
{
public:
	PropertyListContainer();
	virtual ~PropertyListContainer();

	bool has_property_list(const std::string& name);

	template<class T> PropertyList<T> add_list(const std::string& name);
	template<class T> PropertyList<T> add_list(const std::string& name, const UserData &user_data);
	void add_list(std::shared_ptr<IPropertyList> property);

	template<class T> PropertyList<T> get_list(const std::string& name);
	std::shared_ptr<IPropertyList> get_list_interface(const std::string& name);
	std::unordered_map<std::string, std::shared_ptr<IPropertyList>> &get_property_lists();

	void remove_property_list(const std::string& name, bool postpone_delete = false);
	void remove_property_list(const std::string& name, const UserData &user_data, bool postpone_delete = false);
	void remove_all_property_lists();

	void clear_deleted_property_lists();
	void clear_dirty_property_lists();

	void update_property_lists();

	PropertyListContainer &operator= (const PropertyListContainer &rhs);

	clan::Signal<std::shared_ptr<IPropertyList>> &property_list_added();
	clan::Signal<std::shared_ptr<IPropertyList>, const UserData&> &property_list_with_user_data_added();
	clan::Signal<std::shared_ptr<IPropertyList>> &property_list_removed();
	clan::Signal<std::shared_ptr<IPropertyList>, const UserData&> &property_list_with_user_data_removed();

protected:
	std::unordered_map<std::string, std::shared_ptr<IPropertyList>> property_lists;
	std::vector<std::shared_ptr<IPropertyList>> deleted_property_lists;

	clan::Signal<std::shared_ptr<IPropertyList>> sign_PropertyListAdded;
	clan::Signal<std::shared_ptr<IPropertyList>, const UserData&> sign_PropertyListWithUserDataAdded;
	clan::Signal<std::shared_ptr<IPropertyList>> sign_PropertyListRemoved;
	clan::Signal<std::shared_ptr<IPropertyList>, const UserData&> sign_PropertyListWithUserDataRemoved;
};

#include "PropertyListContainer.inl"

} // namespace Framework
} // namespace GM
