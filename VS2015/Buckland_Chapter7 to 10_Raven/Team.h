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
	void addBot(Raven_Bot* bot) { m_teamBots.push_back(bot); }

public:

	Team();
	~Team();

	virtual bool HandleMessage(const Telegram& msg) = 0;

};
#endif // !TEAM_H