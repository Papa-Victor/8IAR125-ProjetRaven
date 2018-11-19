#ifndef Leader_Single_Target_Team_H
#define Leader_Single_Target_Team_H

#include "Single_Target_Team.h"
#include "../Common/Time/Regulator.h"

class Leader_Single_Target_Team : public Single_Target_Team
{
protected:
	Raven_Bot* m_pLeader;

public:
	Leader_Single_Target_Team() : m_pLeader(NULL) { m_currentTarget = NULL; }
	Leader_Single_Target_Team(Raven_Bot* leader) : m_pLeader(leader){ m_currentTarget = NULL; }
	~Leader_Single_Target_Team(){}

	void GiveCurrentTarget() override {
		for (std::list<Raven_Bot*>::iterator it = m_teamBots.begin(); it != m_teamBots.end(); it++) {
			if (*it != m_pLeader) {
				(*it)->GetTargetSys()->SetTarget(m_currentTarget);
			}
		}
	}
};
#endif // !Leader_Single_Target_Team_H