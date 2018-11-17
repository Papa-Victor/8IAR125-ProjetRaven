#ifndef Brain_Leader_Single_Target_Tea_h1
#define Brain_Leader_Single_Target_Tea_h1

#include "Leader_Single_Target_Team.h"

class Brain_Leader_Single_Target_Team : public Leader_Single_Target_Team
{
public:
	Brain_Leader_Single_Target_Team(){}
	Brain_Leader_Single_Target_Team(Raven_Bot* leader) : Leader_Single_Target_Team(leader){}
	~Brain_Leader_Single_Target_Team();

	void AddBot(Raven_Bot* bot) override;
	void RemoveBot(Raven_Bot* bot) override;
	void SetLeader(Raven_Bot* bot) override;
	void UpdateTargetting() override;
	void DetermineTarget() override;
};
#endif // !Brain_Leader_Single_Target_Tea_h1