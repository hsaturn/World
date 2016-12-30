#ifndef _ITEM_H_
#define _ITEM_H_

#include "object.h"
#include "get.h"
#include "bot_queue.h"
#include <string>
#include <map>

using namespace std;

class CFileParser;

class Item : public Object<Item>
{
	friend class Object<Item>;
private:
	Get* mpoGet;
	int miEmus;

public:
	static string getClass() { return "item"; }

	virtual ~Item();

	static bool getSome(const string& sName, int iCount, BotQueue&);
	bool pushActions(BotQueue &);

private:
	Item(const Item&);
	Item& operator=(const Item&);

protected:
	Item(const string& sName);

	virtual bool parseSpecific(CFileParser*, const string &sEntry) ;
	virtual bool hasDefinition() const { return true; }
	virtual void factoryCheck(CFileParser*);
};
#endif
