#ifndef SINGLE_TARGET_TEAM_H
#define SINGLE_TARGET_TEAM_H

#include "Team.h"

class Single_Target_Team : public Team
{
protected:
	Raven_Bot* m_currentTarget;

	void GiveCurrentTarget();

	virtual void DetermineTarget() = 0;

public:
	Single_Target_Team();
	virtual ~Single_Target_Team();

	void CheckDeadBot(const Raven_Bot* bot) override;
};
#endif // !SINGLE_TARGET_TEAM_H