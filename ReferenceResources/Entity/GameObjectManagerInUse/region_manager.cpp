#include "precomp.h"
#include "region_manager.h"
#include "definitions_gameobjects.h"
#include "company_manager.h"
#include "Components/Regions/region.h"
#include "Components/Company/company.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

RegionManager::RegionManager(const ZoneManagerPtr &zone_manager, const GameObjectManagerPtr &gameobject_manager, const CompanyManagerPtr &company_manager)
: zone_manager(zone_manager), gameobject_manager(gameobject_manager), company_manager(company_manager)
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////b
// Operations:

void RegionManager::start_region(const PlayerPtr &player, const ServerGameObjectPtr &company_gameobject, int region_id)
{
	auto company = company_gameobject->getComponent<Company>();
	if (company->get_current_zone_id() == 0)
	{
		auto regions = company_gameobject->find_child_by_type(GAMEOBJECTTYPE_REGIONS);
		if(regions)
		{
			auto region = regions->find_child(region_id);
			if(region)
			{
				log_event("NetEvents", "Starting region %1", region_id);

				auto region_component = region->getComponent<Region>();
				auto zone_id = region_component->get_zone_id();

				company_manager->move_company_to_zone(player, zone_id, nullptr);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

