#include "Random_Single_Target_Team.h"

#include <random>
#include <vector>

void Random_Single_Target_Team::DetermineTarget()
{
	std::list<Raven_Bot*> gameBotsList = game->GetAllBots();
	std::vector<Raven_Bot*> availableBots;

	for (std::list<Raven_Bot*>::iterator it = gameBotsList.begin(); it != gameBotsList.end(); it++) {
		if (!BotInTeam(*it)) {
			availableBots.push_back(*it);
		}
	}
	if (availableBots.size() != 0) {
		m_currentTarget = availableBots[rand() % availableBots.size()];
	}
	else
	{
		m_currentTarget = NULL;
	}
	GiveCurrentTarget();
}


Random_Single_Target_Team::~Random_Single_Target_Team()
{
}

void Random_Single_Target_Team::AddBot(Raven_Bot * bot)
{
	m_teamBots.push_back(bot);
	bot->SetTeam(this);
	if (bot == m_currentTarget || m_currentTarget == NULL) {
		DetermineTarget();
	}
	bot->SetTargetControl(false);
	bot->GetTargetSys()->SetTarget(m_currentTarget);
}

void Random_Single_Target_Team::RemoveBot(Raven_Bot * bot)
{
	m_teamBots.remove(bot);
	bot->SetTeam(NULL);
	bot->SetTargetControl(true);
}
