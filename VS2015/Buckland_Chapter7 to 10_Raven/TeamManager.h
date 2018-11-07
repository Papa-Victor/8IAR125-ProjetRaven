#ifndef TEAM_MANAGER_H
#define TEAM_MANAGER_H

#include "Team.h"
#include "Raven_Game.h"
#include <array>

enum teams
{
	RED,
	BLUE,
	GREEN
};

class TeamManager
{
private:
	TeamManager();

	static TeamManager* instance;

	std::array<Team*, 3> m_Teams;

public:
	~TeamManager() {}
	TeamManager(const TeamManager&) = delete;
	TeamManager& operator=(const TeamManager&) = delete;

	static TeamManager* Instance();

	template <class T>
	void AddTeam(teams colour, Raven_Game* game);
	void RemoveTeam(teams colour) { delete m_Teams[colour]; m_Teams[colour] = NULL; }
	void AddBot(teams colour, Raven_Bot* bot);
	void RemoveBot(teams colour, Raven_Bot* bot);
	void NewWorldBot(Raven_Bot* addedBot);

	void OnBotDeath(Raven_Bot* bot);
};
#endif // !TEAM_MANAGER_H

template<class T>
inline void TeamManager::AddTeam(teams colour, Raven_Game* game)
{
	if (m_Teams[colour] == NULL) {
		delete m_Teams[colour];
	}
	m_Teams[colour] = new T(game);
}
