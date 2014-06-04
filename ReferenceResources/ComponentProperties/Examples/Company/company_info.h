#pragma once

#include "../definitions_component_names.h"
#include <ServerEngine/GameWorld/server_gameobject.h>

class CompanyInfo : public Totem::Component<CompanyInfo>
{
// Construction:
public:
	CompanyInfo(ServerGameObjectPtr owner, const std::string &name = std::string());

	virtual std::string getType() const { return getStaticType(); }
	static std::string getStaticType() { return COMPONENT_COMPANY_INFO; }

// Attributes:
public:
	int get_user_id() const;
	int get_company_id() const;

// Operations:
public:
	void set_user_id(int user_id);
	void set_company_id(int company_id);

// Implementation:
private:
	Totem::Property<int> property_user_id;
	Totem::Property<int> property_company_id;
};
typedef std::shared_ptr<CompanyInfo> CompanyInfoPtr;
