#include "Random_Single_Target_Team.h"

#include <random>
#include <vector>

void Random_Single_Target_Team::DetermineTarget()
{
	std::vector<Raven_Bot*> gameBotsVector;
	std::list<Raven_Bot*> gameBotsList = game->GetAllBots();
	for (std::list<Raven_Bot*>::iterator it = gameBotsList.begin(); it != gameBotsList.end(); it++) {
		gameBotsVector.push_back(*it);
	}
	do {
		m_currentTarget = gameBotsVector[rand() % gameBotsVector.size()];
	} while (BotInTeam(m_currentTarget) || m_currentTarget->isDead());
	GiveCurrentTarget();
}


Random_Single_Target_Team::~Random_Single_Target_Team()
{
}
