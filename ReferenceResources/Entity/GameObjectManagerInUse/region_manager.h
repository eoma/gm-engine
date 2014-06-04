#pragma once

#include <ServerEngine/GameWorld/server_gameobject.h>
#include <ServerEngine/GameWorld/Zone/zone_manager.h>
#include <ServerEngine/GameWorld/gameobject_manager.h>

class CompanyManager; typedef std::shared_ptr<CompanyManager> CompanyManagerPtr;

class RegionManager
{
// Construction:
public:
	RegionManager(const ZoneManagerPtr &zone_manager, const GameObjectManagerPtr &gameobject_manager, const CompanyManagerPtr &company_manager);

// Attributes:
public:

// Operations:
public:
	void start_region(const PlayerPtr &player, const ServerGameObjectPtr &company_gameobject, int region_id);

// Implementation:
private:
	ZoneManagerPtr zone_manager;
	GameObjectManagerPtr gameobject_manager;
	CompanyManagerPtr company_manager;
};

typedef std::shared_ptr<RegionManager> RegionManagerPtr;
