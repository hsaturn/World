#ifndef _BOT_ACTION_BUY_H_
#define _BOT_ACTION_BUY_H_

#include "action.h"

class CFileParser;

class ActionBuy : public Action
{
public:
public:
	ActionBuy(CFileParser*);
	virtual ~ActionBuy();

	virtual bool pushActions(BotQueue&);

protected:
	virtual bool parseSpecific(CFileParser*, const string &sEntry);

private:
	int miPcb;
};

#endif
