#ifndef _BOT_ACTION_HARVEST_H_
#define _BOT_ACTION_HARVEST_H_

#include "action.h"

class CFileParser;

class ActionHarvest : public Action
{
public:
	ActionHarvest(CFileParser *);
	virtual ~ActionHarvest(){};

	virtual bool pushActions(BotQueue&);

protected:
	virtual bool parseSpecific(CFileParser*, const string &sEntry);

};

#endif
