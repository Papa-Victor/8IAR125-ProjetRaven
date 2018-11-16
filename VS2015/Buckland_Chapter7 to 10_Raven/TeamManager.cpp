#include "TeamManager.h"
#include "Raven_WeaponSystem.h"

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

Vector2D TeamManager::GetDroppedWeaponPosition(int team, int weaponType)
{
	switch (team) {
	case teams::RED:
		switch (weaponType) {
		case type_shotgun:
			return Vector2D(150, 20);
		case type_rocket_launcher:
			return Vector2D(130, 20);
		case type_rail_gun:
			return Vector2D(150, 40);
		}
	case teams::BLUE:
		switch (weaponType) {
		case type_shotgun:
			return Vector2D(342, 40);
		case type_rocket_launcher:
			return Vector2D(360, 40);
		case type_rail_gun:
			return Vector2D(378, 42);
		}
	case teams::GREEN:
		switch (weaponType) {
		case type_shotgun:
			return Vector2D(470, 180);
		case type_rocket_launcher:
			return Vector2D(470, 165);
		case type_rail_gun:
			return Vector2D(470, 150);
		}
	}
}

void TeamManager::TryDroppedWeapons(std::list<Raven_Bot*> botList)
{
	for (std::array<Team*, 3>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++) {
		if (*it != NULL) {
			(*it)->TryDroppedWeapons(botList);
		}
	}
}

void TeamManager::RenderDroppedWeapons()
{
	for (std::array<Team*, 3>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++) {
		if (*it != NULL) {
			(*it)->RenderDroppedWeapons();
		}
	}
}


void TeamManager::OnBotDeath(Raven_Bot * bot)
{
	Team* curTeam;
	for (int i = 0; i < m_Teams.size(); i++) {
		curTeam = m_Teams[i];
		if (curTeam != NULL) {
			if (curTeam->BotInTeam(bot)) {
				curTeam->AddDroppedWeapon(type_shotgun, GetDroppedWeaponPosition(i, type_shotgun), game->GetMap()->GetNavGraph(), bot);
				curTeam->AddDroppedWeapon(type_rocket_launcher, GetDroppedWeaponPosition(i, type_rocket_launcher), game->GetMap()->GetNavGraph(), bot);
				curTeam->AddDroppedWeapon(type_rail_gun, GetDroppedWeaponPosition(i, type_rail_gun), game->GetMap()->GetNavGraph(), bot);
			}
			curTeam->CheckDeadBot(bot);

		}

	}
}
