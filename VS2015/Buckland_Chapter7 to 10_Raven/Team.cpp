#include "Team.h"



bool Team::BotInTeam(Raven_Bot const *bot) const
{
	for (std::list<Raven_Bot*>::const_iterator it = m_teamBots.begin(); it != m_teamBots.end(); it++) {
		if (bot == *it) {
			return true;
		}
	}
	return false;
}

Team::Team()
{
}


Team::~Team()
{
	m_teamBots.clear();
}
