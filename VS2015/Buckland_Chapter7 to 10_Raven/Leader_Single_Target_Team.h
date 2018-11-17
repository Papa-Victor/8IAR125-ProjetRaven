#ifndef Leader_Single_Target_Team_H
#define Leader_Single_Target_Team_H

#include "Single_Target_Team.h"

class Leader_Single_Target_Team : public Single_Target_Team
{
protected:
	Raven_Bot* m_pLeader;

public:
	Leader_Single_Target_Team() : m_pLeader(NULL) { m_currentTarget = NULL; }
	Leader_Single_Target_Team(Raven_Bot* leader) : m_pLeader(leader){ m_currentTarget = NULL; }
	~Leader_Single_Target_Team(){}
};
#endif // !Leader_Single_Target_Team_H