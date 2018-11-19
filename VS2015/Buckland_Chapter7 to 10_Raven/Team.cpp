#include "Team.h"
#include "Trigger_TeamWeaponGiver.h"
#include "Raven_WeaponSystem.h"
#include "TeamManager.h"

bool Team::BotInTeam(Raven_Bot const *bot) const
{
	if (bot->GetTeam() == this) {
		return true;
	}
	return false;
}

void Team::AddDroppedWeapon(int weaponType, Vector2D pos, NavGraph& navGraph, Raven_Bot* pBot)
{
	if (pBot->GetWeaponSys()->GetWeaponFromInventory(weaponType) != NULL) {
		Trigger<Raven_Bot> *wg = new Trigger_TeamWeaponGiver(pos, pBot);

		wg->SetEntityType(weaponType);

		//add it to the appropriate vectors
		m_DroppedWeapons.push_back(wg);

		//let the corresponding navgraph node point to this object
		NavGraph::NodeType& node = navGraph.GetNode(wg->GraphNodeIndex());

		int graphNodeIndex = wg->GraphNodeIndex();

		node.SetExtraInfo(wg);

		//register the entity 
		EntityMgr->RegisterEntity(wg);
	}
}

void Team::TryDroppedWeapons(std::list<Raven_Bot*> botList)
{
	for (std::list<Raven_Bot*>::iterator curBot = botList.begin(); curBot != botList.end(); curBot++) {
		for (Raven_Map::TriggerSystem::TriggerList::iterator curTrigger = m_DroppedWeapons.begin(); curTrigger != m_DroppedWeapons.end(); curTrigger++) {
			(*curTrigger)->Try(*curBot);
		}
	}
}

void Team::RenderTeamCircles(teams team)
{
	switch (team) {
	case teams::RED:
		gdi->ThickRedPen(); gdi->HollowBrush();
		for (std::list<Raven_Bot*>::iterator curBot = m_teamBots.begin(); curBot != m_teamBots.end(); curBot++) {
			gdi->Circle((*curBot)->Pos(), (*curBot)->BRadius() + 3);
		}
		break;
	case teams::BLUE:
		gdi->ThickBluePen(); gdi->HollowBrush();
		for (std::list<Raven_Bot*>::iterator curBot = m_teamBots.begin(); curBot != m_teamBots.end(); curBot++) {
			gdi->Circle((*curBot)->Pos(), (*curBot)->BRadius() + 3);
		}
		break;
	case teams::GREEN:
		gdi->ThickGreenPen(); gdi->HollowBrush();
		for (std::list<Raven_Bot*>::iterator curBot = m_teamBots.begin(); curBot != m_teamBots.end(); curBot++) {
			gdi->Circle((*curBot)->Pos(), (*curBot)->BRadius() + 3);
		}
		break;
	}
}

void Team::RenderDroppedWeapons()
{
	for (Raven_Map::TriggerSystem::TriggerList::iterator curTrigger = m_DroppedWeapons.begin(); curTrigger != m_DroppedWeapons.end(); curTrigger++) {
		(*curTrigger)->Render();
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
