#ifndef _BOT_ACTION_H_
#define _BOT_ACTION_H_

#include <string>
using namespace std;

#include "bot_queue.h"
#include <list>
using namespace std;

class CFileParser;
class Item;

class ActionItem
{
private:
	Item* mpoItem;
	int miCount;

public:
	ActionItem(Item* pItem, int iCount) : mpoItem(pItem),miCount(iCount){}

	int getCount() const { return miCount; }
	Item* getItem() const { return mpoItem; }
};

class Action
{
protected:
	typedef list<ActionItem*>		TItems;
	typedef TItems::const_iterator	const_iterator;
	typedef TItems::iterator		iterator;

private:
	TItems	lstItems;

public:
	const_iterator	begin() const { return lstItems.begin(); }
	const_iterator	end() const	{ return lstItems.end(); }
	iterator		begin() { return lstItems.begin(); }
	iterator		end()	{ return lstItems.end(); }

public:
	Action(){};
	virtual ~Action();

	virtual bool pushActions(BotQueue&)=0;

protected:
	void parse(CFileParser*, const string &sActionName);
	virtual bool parseSpecific(CFileParser*, const string &sEntry){ return false; };
};

#endif
