#pragma once

#include <memory>
#include <string>

namespace Totem
{

class IProperty
{
public:
	virtual const std::string &getName() const = 0;
	virtual bool isNull() const = 0;
	virtual bool isDirty() const = 0;
	virtual void clearDirty() = 0;

	template<typename PropertyType> static bool isType(const std::shared_ptr<IProperty> &property);
	template<typename PropertyType> static bool isType(const IProperty &property);
	virtual unsigned int getRuntimeTypeId() const = 0;
	template<typename PropertyType> static unsigned int getRuntimeTypeId();
};

#include "IProperty.inl"

} //namespace Totem

