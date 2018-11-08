#include "TeamManager.h"

TeamManager* TeamManager::instance = NULL;



TeamManager::TeamManager(Raven_Game* game)
{
	this->game = game;
	for (std::array<Team*, 3>::iterator curTeam = m_Teams.begin(); curTeam != m_Teams.end(); curTeam++) {
		*curTeam = NULL;
	}
}

TeamManager * TeamManager::Instance(Raven_Game* game)
{
	if (instance == NULL) {
		instance = new TeamManager(game);
	}
	return instance;
}

void TeamManager::AddBot(teams colour, Raven_Bot * bot)
{
	Team* originalTeam = bot->GetTeam();
	if (originalTeam != NULL) {
		originalTeam->RemoveBot(bot);
	}
	if (!m_Teams[colour]->BotInTeam(bot)) {
		m_Teams[colour]->AddBot(bot);
	}
}

void TeamManager::RemoveBot(teams colour, Raven_Bot * bot)
{
	m_Teams[colour]->RemoveBot(bot);
}

void TeamManager::NewWorldBot(Raven_Bot * addedBot)
{
	for (std::array<Team*, 3>::iterator curTeam = m_Teams.begin(); curTeam != m_Teams.end(); curTeam++) {
		if (*curTeam != NULL) {
			(*curTeam)->NewWorldBot(addedBot);
		}
	}
}

void TeamManager::OnBotDeath(Raven_Bot * bot)
{
	for (std::array<Team*, 3>::iterator curTeam = m_Teams.begin(); curTeam != m_Teams.end(); curTeam++) {
		if (*curTeam != NULL) {
			(*curTeam)->CheckDeadBot(bot);
		}
	}
}
