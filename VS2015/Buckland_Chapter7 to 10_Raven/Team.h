#ifndef TEAM_H
#define TEAM_H

#include "Raven_Bot.h"
#include "Raven_Map.h"
#include "Game/EntityManager.h"
#include <list>

enum teams;

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

	void AddDroppedWeapon(int weaponType, Vector2D pos, NavGraph& navGraph, Raven_Bot* pBot);
	void TryDroppedWeapons(std::list<Raven_Bot*> botList);
	void RenderTeamCircles(teams team);
	void RenderDroppedWeapons();
	Raven_Map::TriggerSystem::TriggerList& GetDroppedWeapons() { return m_DroppedWeapons; }

	virtual void AddBot(Raven_Bot* bot) = 0;
	virtual void RemoveBot(Raven_Bot* bot) = 0;
	virtual void CheckDeadBot(const Raven_Bot* bot){}
	virtual void NewWorldBot(const Raven_Bot* bot){}
	virtual void SetLeader(Raven_Bot* bot) {}
	virtual void UpdateTargetting(){}


};
#endif // !TEAM_H