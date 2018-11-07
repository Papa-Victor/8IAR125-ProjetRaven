#ifndef Random_Single_Target_Team_H
#define Random_Single_Target_Team_h

#include "Single_Target_Team.h"
#include "Raven_Game.h"

class Random_Single_Target_Team : public Single_Target_Team
{
private:
	void DetermineTarget() override;

	Raven_Game* game;

public:
	Random_Single_Target_Team(Raven_Game* game) { this->game = game; }
	~Random_Single_Target_Team();

	void AddBot(Raven_Bot* bot) override;
	void RemoveBot(Raven_Bot* bot) override;
	void NewWorldBot(const Raven_Bot* bot) override;
};
#endif // !Random_Single_Target_Team