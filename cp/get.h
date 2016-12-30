#ifndef _BOT_GET_H_
#define _BOT_GET_H_

#include "action.h"
#include "bot_queue.h"

class CFileParser;

class Get
{
private:
	Action*	moAction;

public:
	Get(CFileParser*);
	~Get();

	void pushActions(BotQueue&);
	Action* getAction();
};

#endif
