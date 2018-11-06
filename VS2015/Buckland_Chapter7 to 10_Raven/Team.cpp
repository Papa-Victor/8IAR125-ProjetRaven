#include "Team.h"



bool Team::BotInTeam(Raven_Bot const *bot) const
{
	if (bot->GetTeam() == this) {
		return true;
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
