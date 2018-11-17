#include "Normal_Leader_Single_Target_Team.h"



Normal_Leader_Single_Target_Team::~Normal_Leader_Single_Target_Team()
{
	m_currentTarget = NULL;
	m_pLeader = NULL;
	while (!m_teamBots.empty()) {
		RemoveBot(m_teamBots.back());
	}
}

void Normal_Leader_Single_Target_Team::AddBot(Raven_Bot * bot)
{
	m_teamBots.push_back(bot);
	bot->SetTeam(this);
	bot->SetTargetControl(false);
	bot->GetTargetSys()->SetTarget(m_currentTarget);
	if (m_pLeader == NULL) {
		SetLeader(bot);
	}
}

void Normal_Leader_Single_Target_Team::RemoveBot(Raven_Bot * bot)
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

void Normal_Leader_Single_Target_Team::SetLeader(Raven_Bot * bot)
{
	if (bot != m_pLeader) {
		if (m_pLeader != NULL) {
			m_pLeader->SetTargetControl(false);
		}
		if (!BotInTeam(bot)) {
			AddBot(bot);
		}
		m_pLeader = bot;
		m_pLeader->SetTargetControl(true);
		m_currentTarget = m_pLeader->GetTargetSys()->GetTarget();
	}
}

void Normal_Leader_Single_Target_Team::UpdateTargetting()
{
	if (m_pLeader != NULL && m_pLeader->GetTargetSelectionRegulator()->isReady()) {
		m_currentTarget = m_pLeader->GetTargetSys()->GetTarget();
		GiveCurrentTarget();
	}
}