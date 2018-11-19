#ifndef Normal_Leader_Single_Target_Team_h
#define Normal_Leader_Single_Target_Team_h

#include "Leader_Single_Target_Team.h"

class Normal_Leader_Single_Target_Team : public Leader_Single_Target_Team
{
public:
	Normal_Leader_Single_Target_Team(){}
	Normal_Leader_Single_Target_Team(Raven_Bot* leader) : Leader_Single_Target_Team(leader){}
	~Normal_Leader_Single_Target_Team();

	void AddBot(Raven_Bot* bot) override;
	void RemoveBot(Raven_Bot* bot) override;
	void SetLeader(Raven_Bot* bot) override;
	void UpdateTargetting() override;
	void DetermineTarget() override {}
};
#endif // !Normal_Leader_Single_Target_Team_h