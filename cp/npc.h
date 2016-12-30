#ifndef _BOT_NPC_H_
#define _BOT_NPC_H_

#include "object.h"

class CFileParser;
class Coord;

class Npc : public Object<Npc>
{
	friend class Object<Npc>;

private:
	Coord*	pos;
	bool mbStorage;

public:
	static string getClass() { return "npc"; }
	virtual ~Npc();

	Coord* getCoords() { return pos; }

private:
	void setStorage(CFileParser* );
	Npc(const Npc&);
	Npc& operator=(const Npc&);

protected:
	Npc(const string& sName);

	virtual bool parseSpecific(CFileParser*, const string &sEntry) ;
	virtual bool hasDefinition() const { return true; }
	virtual void postFactory(CFileParser*);
};

#endif
