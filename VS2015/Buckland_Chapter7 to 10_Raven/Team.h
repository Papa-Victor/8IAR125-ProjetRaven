#ifndef TEAM_H
#define TEAM_H

#include "Raven_Bot.h"
#include "Raven_Map.h"
#include "Game/EntityManager.h"
#include <list>

typedef NavGraphNode<Trigger<Raven_Bot>*>         GraphNodeX;
typedef SparseGraph<GraphNodeX, NavGraphEdge>      NavGraph;

class Team
{
protected:
	std::list<Raven_Bot*> m_teamBots;

	Raven_Map::TriggerSystem::TriggerList m_DroppedWeapons;


public:

	Team(){}
	virtual ~Team();


	bool BotInTeam(Raven_Bot const *bot)const;

	void AddDroppedWeapon(int weaponType, Vector2D pos, NavGraph navGraph, Raven_Bot* pBot);
	Raven_Map::TriggerSystem::TriggerList& GetDroppedWeapons() { return m_DroppedWeapons; }

	virtual void AddBot(Raven_Bot* bot) = 0;
	virtual void RemoveBot(Raven_Bot* bot) = 0;
	virtual void CheckDeadBot(const Raven_Bot* bot){}
	virtual void NewWorldBot(const Raven_Bot* bot){}

};
#endif // !TEAM_H