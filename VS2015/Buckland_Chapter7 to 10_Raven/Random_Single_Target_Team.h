#ifndef Random_Single_Target_Team_H
#define Random_Single_Target_Team_h

#include "Single_Target_Team.h"
#include "Raven_Game.h"

class Random_Single_Target_Team : Single_Target_Team
{
private:
	void DetermineTarget() override;

	Raven_Game* game;

public:
	Random_Single_Target_Team(Raven_Game* game) { this->game = game; }
	~Random_Single_Target_Team();
};
#endif // !Random_Single_Target_Team