#pragma once
#include <ClanLib/core.h>

 
 

namespace GM {
namespace Framework {

class IProperty
{
public:
	virtual const std::string &get_name() const = 0;
	virtual bool is_null() const = 0;
	virtual bool is_dirty() const = 0;
	virtual void clear_dirty() = 0;

	template<typename PropertyType> static bool is_type(const std::shared_ptr<IProperty> &property);
	template<typename PropertyType> static bool is_type(const IProperty &property);
	virtual size_t get_runtime_type_id() const = 0;
	template<typename PropertyType> static size_t get_runtime_type_id();
};

#include "IProperty.inl"

} // namespace Framework
} // namespace GM
