#ifndef TEAM_H
#define TEAM_H

#include "Raven_Bot.h"
#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include <list>



class Team
{
protected:
	std::list<Raven_Bot*> m_teamBots;

	bool BotInTeam(Raven_Bot const *bot)const;

public:

	Team();
	~Team();

	virtual void AddBot(Raven_Bot* bot) = 0;
	virtual void RemoveBot(Raven_Bot* bot) = 0;
	virtual void CheckDeadBot(const Raven_Bot* bot){}

};
#endif // !TEAM_H