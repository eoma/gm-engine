#pragma once

#include <ServerEngine/GameWorld/player.h>
#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/gameobject_manager.h>
#include <ServerEngine/GameWorld/Zone/zone.h>
#include <ServerEngine/GameWorld/Zone/zone_manager.h>

class TemplateManager; typedef std::shared_ptr<TemplateManager> TemplateManagerPtr;

class CompanyManager
{
// Enums:
public:

// Construction:
public:
	CompanyManager(const ServerGameObjectPtr &world,
					 const TemplateManagerPtr &template_manager,
					 const ZoneManagerPtr &zone_manager, 
					 const GameObjectManagerPtr &gameobject_manager);

	void init(std::function<void()> func);

// Attributes:
public:
	ServerGameObjectPtr find_company_gameobject(const PlayerPtr &player);
	
	std::vector<ServerGameObjectPtr> get_available_companies(int user_id);

	bool is_company_name_available(const std::string &name);

// Operations:
public:
	void login_company(int company_id, const PlayerPtr &player);
	void logout_company(const PlayerPtr &player);
	
	void ready_company(const PlayerPtr &player);
	void create_company(int user_id, const std::string &company_name, const std::function<void(ServerGameObjectPtr, ServerGameObjectPtr)> &func);

	void move_company_to_zone(const PlayerPtr &player, int zone_id, const std::function<void()> &func);

// Implementation:
private:
	void initialize_company(const ServerGameObjectPtr &company_info_gameobject, const PlayerPtr &player);

	ZonePtr world_zone;
	TemplateManagerPtr template_manager;
	ZoneManagerPtr zone_manager;
	ServerGameObjectPtr company_infos;
	GameObjectManagerPtr gameobject_manager;

	std::map<PlayerPtr, ServerGameObjectPtr> logged_in_companies;
};

typedef std::shared_ptr<CompanyManager> CompanyManagerPtr;
