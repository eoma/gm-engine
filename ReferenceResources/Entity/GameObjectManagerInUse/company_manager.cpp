#include "precomp.h"
#include "company_manager.h"
#include "definitions_template_names.h"
#include "definitions_gameobjects.h"
#include "Components/Company/company.h"
#include "Components/Company/company_info.h"
#include "Components/tile_map.h"
#include "NetEvents/definitions_netevents.h"
#include <ServerEngine/GameWorld/Components/template_manager.h>
#include <ServerEngine/GameWorld/Components/replicated_container.h>

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CompanyManager::CompanyManager(const ServerGameObjectPtr &world,
								   const TemplateManagerPtr &template_manager,
								   const ZoneManagerPtr &zone_manager, 
								   const GameObjectManagerPtr &gameobject_manager)
: template_manager(template_manager),
  zone_manager(zone_manager),
  gameobject_manager(gameobject_manager)
{
	world_zone = zone_manager->get_or_create_zone(world, Zone::KEEP_WHEN_EMPTY);
}

void CompanyManager::init(std::function<void()> func)
{
	gameobject_manager->get_or_load_gameobject(GAMEOBJECTID_COMPANIES, 
		[this, func](ServerGameObjectPtr company_infos)
		{
			this->company_infos = company_infos;
			if(func != nullptr) func();
		}
	);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

ServerGameObjectPtr CompanyManager::find_company_gameobject(const PlayerPtr &player)
{
	auto it = logged_in_companies.find(player);
	if (it != logged_in_companies.end())
		return it->second;
	else
		return nullptr;
}

std::vector<ServerGameObjectPtr> CompanyManager::get_available_companies(int user_id)
{
	std::vector<ServerGameObjectPtr> available_company_infos;

	auto all_company_infos = company_infos->get_children();
	for (auto it = all_company_infos.begin(); it != all_company_infos.end(); ++it)
	{
		auto company_info_gameobject = (*it);
		auto company_info = (*it)->getComponent<CompanyInfo>();
		if (company_info->get_user_id() == user_id)
		{
			available_company_infos.push_back(company_info_gameobject);
		}
	}
	return available_company_infos;
}

bool CompanyManager::is_company_name_available(const std::string &name)
{
	auto all_company_infos = company_infos->get_children();
	for (auto it = all_company_infos.begin(); it != all_company_infos.end(); ++it)
	{
		auto company_info = (*it);
		if (StringHelp::text_to_lower(company_info->get_name()) == StringHelp::text_to_lower(name))
			return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CompanyManager::login_company(int requested_company_id, const PlayerPtr &player)
{
	auto company_infos = get_available_companies(player->get_user_id());
	for (size_t i = 0; i < company_infos.size(); ++i)
	{
		if (!company_infos[i]->hasComponent<CompanyInfo>())
			continue;

		//auto company_info = company_infos[i]->getComponent<CompanyInfo>();
		if (company_infos[i]->get_id() == requested_company_id)
		{
			initialize_company(company_infos[i], player);
			return;
		}
	}

	player->send_event(NetGameEvent(STC_COMPANY_LOGIN_FAIL, string_format("Company %1 not found", requested_company_id)));
}

void CompanyManager::logout_company(const PlayerPtr &player)
{
	ServerGameObjectPtr company_gameobject = logged_in_companies[player];
	if (company_gameobject != nullptr)
	{
		log_event("Debug", "Logging out company");
		auto company = company_gameobject->getComponent<Company>();
		auto zone = company->get_current_zone();
		if(zone != nullptr)
			zone->get_container_gameobject()->remove_child(company_gameobject, ServerGameObject::STORAGE_TEMPORARY);

		logged_in_companies.erase(player);
	}
}

void CompanyManager::create_company(int user_id, const std::string &company_name, const std::function<void(ServerGameObjectPtr, ServerGameObjectPtr)> &func)
{
	gameobject_manager->create_gameobject(GAMEOBJECTTYPE_REGIONS, "Regions",
		[this, func, user_id, company_name](ServerGameObjectPtr regions)
		{
			gameobject_manager->create_gameobject(GAMEOBJECTTYPE_COMPANY, company_name,
				[this, func, user_id, regions](ServerGameObjectPtr company_gameobject)
				{
					template_manager->apply(TEMPLATE_COMPANY, company_gameobject);

					company_gameobject->add_child(regions, ServerGameObject::STORAGE_PERMANENT);

					auto company = company_gameobject->getComponent<Company>();
					company->set_current_zone_id(0);

					gameobject_manager->create_gameobject(GAMEOBJECTTYPE_COMPANY_INFO, company_gameobject->get_name(),
						[this, func, user_id, company_gameobject](ServerGameObjectPtr company_info_gameobject)
						{
							template_manager->apply(TEMPLATE_COMPANY_INFO, company_info_gameobject);

							auto company_info = company_info_gameobject->getComponent<CompanyInfo>();
							company_info->set_user_id(user_id);
							company_info->set_company_id(company_gameobject->get_id());
							company_info_gameobject->add_child(company_gameobject, ServerGameObject::STORAGE_PERMANENT);

							company_infos->add_child(company_info_gameobject, ServerGameObject::STORAGE_PERMANENT);

							company_info_gameobject->save_dirty_properties(nullptr);

							if (func != nullptr) func(company_gameobject, company_info_gameobject);
						}
					);
				}
			);
		}
	);
}

void CompanyManager::ready_company(const PlayerPtr &player)
{
	auto it = logged_in_companies.find(player);
	if (it == logged_in_companies.end())
		throw Exception(string_format("Player %1 does not have a logged in company", player->get_user_id()));

	auto company_gameobject = it->second;
	auto company = company_gameobject->getComponent<Company>();

	auto regions = zone_manager->get_or_create_zone(company_gameobject->find_child_by_type(GAMEOBJECTTYPE_REGIONS), Zone::DESTROY_WHEN_EMPTY);

    // Subscribe player to data
    world_zone->add_player(player);
    regions->add_player(player);

	player->send_event(NetGameEvent(STC_COMPANY_READY, company_gameobject->get_id()));

    move_company_to_zone(player, company->get_current_zone_id(), nullptr);
}

void CompanyManager::move_company_to_zone(const PlayerPtr &player, int zone_id, const std::function<void()> &func)
{
	if (zone_id == 0)
	{
		if (func != nullptr) func();
		return;
	}

    auto it = logged_in_companies.find(player);
	if (it == logged_in_companies.end())
        throw Exception(string_format("Player %1 does not have a logged in company", player->get_user_id()));

    auto company_gameobject = it->second;
	auto company = company_gameobject->getComponent<Company>();
	auto previous_zone = company->get_current_zone();

    player->send_event(NetGameEvent(STC_ZONE_ENTERING, zone_id));

    gameobject_manager->get_or_load_gameobject(zone_id,
		[this, func, company_gameobject, company, player, previous_zone](ServerGameObjectPtr zone_gameobject)
            {
				auto tilemap = zone_gameobject->getComponent<TileMap>();

				auto zone = zone_manager->get_or_create_zone(zone_gameobject, Zone::DESTROY_WHEN_EMPTY);
                zone->add_player(player);

				if(previous_zone)	// Did we change zone, or just reenter during login?
				{
					company->set_current_zone_id(zone_gameobject->get_id());
				}

				zone_gameobject->add_child(company_gameobject, ServerGameObject::STORAGE_TEMPORARY);

				if(previous_zone && previous_zone->get_id() != zone_gameobject->get_id())
					previous_zone->remove_player(player);

				// TODO: the following feels abit hackish
				player->get_replication_container()->sync_dirty_gameobjects();
				player->get_replication_container()->clear_dirty_gameobjects();

				player->send_event(NetGameEvent(STC_ZONE_ENTERED, zone->get_id()));

                if(func != nullptr) func();
            }
    );
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CompanyManager::initialize_company(const ServerGameObjectPtr &company_info_gameobject, const PlayerPtr &player)
{
	try
	{
		auto company_info = company_info_gameobject->getComponent<CompanyInfo>();
		auto company_id = company_info->get_company_id();

		gameobject_manager->get_or_load_gameobject(company_id,
			[this, player](ServerGameObjectPtr company_gameobject)
			{
				logged_in_companies[player] = company_gameobject;
				auto company_component = company_gameobject->getComponent<Company>();
				company_component->set_player(player);

				player->send_event(NetGameEvent(STC_COMPANY_LOGIN_SUCCESS, company_gameobject->get_id()));
			}
		);
	}
	catch (Exception &e)
	{
		logout_company(player);

		player->send_event(NetGameEvent(STC_COMPANY_LOGIN_FAIL, "Unable to login"));

		log_event("Security", string_format("Failed to login company %1 (%2)", company_info_gameobject->get_id(), e.message));
	}
}
