#include "Brain_Leader_Single_Target_Team.h"
#include "Raven_SensoryMemory.h"

Brain_Leader_Single_Target_Team::~Brain_Leader_Single_Target_Team()
{
	m_currentTarget = NULL;
	m_pLeader = NULL;
	while (!m_teamBots.empty()) {
		RemoveBot(m_teamBots.back());
	}
}

void Brain_Leader_Single_Target_Team::AddBot(Raven_Bot * bot)
{
	m_teamBots.push_back(bot);
	bot->SetTeam(this);
	bot->SetTargetControl(false);
	bot->GetTargetSys()->SetTarget(m_currentTarget);
	if (m_pLeader == NULL) {
		SetLeader(bot);
	}
}

void Brain_Leader_Single_Target_Team::RemoveBot(Raven_Bot * bot)
{
	m_teamBots.remove(bot);
	bot->SetTeam(NULL);
	bot->SetTargetControl(true);
	if (bot == m_pLeader) {
		if (m_teamBots.size() != 0) {
			SetLeader(m_teamBots.front());
		}
		else {
			m_pLeader = NULL;
			m_currentTarget = NULL;
		}
	}
}

void Brain_Leader_Single_Target_Team::SetLeader(Raven_Bot * bot)
{
	if (bot != m_pLeader) {
		m_pLeader = bot;
		m_pLeader->GetTargetSys()->SetTarget(NULL);
		if (!BotInTeam(bot)) {
			AddBot(bot);
		}
		DetermineTarget();
	}
}

void Brain_Leader_Single_Target_Team::UpdateTargetting()
{
	if (m_pLeader != NULL && m_pLeader->GetTargetSelectionRegulator()->isReady()) {
		DetermineTarget();
		GiveCurrentTarget();
	}
}

void Brain_Leader_Single_Target_Team::DetermineTarget()
{
	std::list<Raven_Bot*> recentlySensed = m_pLeader->GetSensoryMem()->GetListOfRecentlySensedOpponents();

	if (recentlySensed.size() != 0) {
		Vector2D teamGravityMass(0, 0);
		for (std::list<Raven_Bot*>::iterator curBot = m_teamBots.begin(); curBot != m_teamBots.end(); curBot++) {
			if (*curBot != m_pLeader) {
				teamGravityMass += (*curBot)->Pos();
			}
		}
		teamGravityMass /= m_teamBots.size();

		double smallestDistance = MaxDouble;

		for (std::list<Raven_Bot*>::iterator curBot = recentlySensed.begin(); curBot != recentlySensed.end(); curBot++) {
			if (!BotInTeam(*curBot)) {
				double currentDistance = (teamGravityMass - (*curBot)->Pos()).Length();

				if (currentDistance < smallestDistance) {
					smallestDistance = currentDistance;
					m_currentTarget = *curBot;
				}
			}
		}
	}
	else {
		m_currentTarget = NULL;
	}
}
