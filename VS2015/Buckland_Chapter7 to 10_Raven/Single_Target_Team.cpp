#include "Single_Target_Team.h"



void Single_Target_Team::GiveCurrentTarget()
{
	for (std::list<Raven_Bot*>::iterator it = m_teamBots.begin(); it != m_teamBots.end(); it++) {
		(*it)->GetTargetSys()->SetTarget(m_currentTarget);
	}
}

Single_Target_Team::Single_Target_Team()
{
	m_currentTarget = NULL;
}


Single_Target_Team::~Single_Target_Team()
{
	m_currentTarget = NULL;
}

void Single_Target_Team::CheckDeadBot(const Raven_Bot * deadBot)
{
	if (m_currentTarget == deadBot) {
		DetermineTarget();
	}
}
