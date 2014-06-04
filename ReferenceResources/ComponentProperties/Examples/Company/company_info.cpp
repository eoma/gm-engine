#include "precomp.h"
#include "company_info.h"
#include "../definitions_property_names.h"

using namespace clan;
using namespace Totem;

CompanyInfo::CompanyInfo(ServerGameObjectPtr owner, const std::string &name)
: Component<CompanyInfo>(name)
{
	property_user_id = owner->add<int>(PROPERTY_USER_ID, 0);
	property_company_id = owner->add<int>(PROPERTY_COMPANY_ID, 0);
}

int CompanyInfo::get_user_id() const
{ 
	return property_user_id;
}

int CompanyInfo::get_company_id() const
{ 
	return property_company_id;
}

void CompanyInfo::set_user_id(int user_id)
{
	property_user_id = user_id;
}

void CompanyInfo::set_company_id(int company_id)
{
	property_company_id = company_id;
}
