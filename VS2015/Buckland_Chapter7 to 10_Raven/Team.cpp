#include "Team.h"
#include "Trigger_TeamWeaponGiver.h"
#include "Raven_WeaponSystem.h"

bool Team::BotInTeam(Raven_Bot const *bot) const
{
	if (bot->GetTeam() == this) {
		return true;
	}
	return false;
}

void Team::AddDroppedWeapon(int weaponType, Vector2D pos, NavGraph navGraph, Raven_Bot* pBot)
{
	if (pBot->GetWeaponSys()->GetWeaponFromInventory(weaponType) != NULL) {
		Trigger_TeamWeaponGiver *wg = new Trigger_TeamWeaponGiver(pos, pBot);

		wg->SetEntityType(weaponType);

		//add it to the appropriate vectors
		m_DroppedWeapons.push_back(wg);

		//let the corresponding navgraph node point to this object
		NavGraph::NodeType& node = navGraph.GetNode(wg->GraphNodeIndex());

		node.SetExtraInfo(wg);

		//register the entity 
		EntityMgr->RegisterEntity(wg);
	}
}

Team::~Team()
{
	m_teamBots.clear();
	for (Raven_Map::TriggerSystem::TriggerList::iterator it = m_DroppedWeapons.begin(); it != m_DroppedWeapons.end(); it++) {
		delete *it;
	}
	m_DroppedWeapons.clear();
}
