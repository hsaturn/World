#ifndef _BOT_ACTION_MIX_H_
#define _BOT_ACTION_MIX_H_

#include "action.h"

class CFileParser;

class ActionMix : public Action
{
public:
public:
	ActionMix(CFileParser*);
	virtual ~ActionMix();

	virtual bool pushActions(BotQueue&);

protected:
	virtual bool parseSpecific(CFileParser*, const string &sEntry);

private:
	int msFoodRequired;
};

#endif
