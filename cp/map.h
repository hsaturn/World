#ifndef _Map_H_
#define _Map_H_

#include "object.h"
#include "get.h"
#include <string>
#include <map>
#include <list>

using namespace std;

class CFileParser;
class Coord;
class Npc;

class Map : public Object<Map>
{
	friend class Object<Map>;
public:
	typedef map<string, list<Coord*> > 		TItemContainer;
	typedef TItemContainer::iterator		item_iterator;
	typedef TItemContainer::const_iterator const_item_iterator;

	typedef map<string, Npc*> 				TNpcContainer;
	typedef TNpcContainer::iterator			npc_iterator;
	typedef TNpcContainer::const_iterator	const_npc_iterator;

	typedef list<Coord*>					TExitContainer;
	typedef TExitContainer::iterator		exit_iterator;
	typedef	 TExitContainer::const_iterator	const_exit_iterator;

private:
	TItemContainer		mapItems;
	TNpcContainer		mapNpcs;
	TExitContainer		lstExits;

	Map*	mpoParentMap;	// If the map is inside another
	bool	mbEntrySet;		// For a nested map, this is used to check if entry was set to the parent map

public:
	static string getClass() { return "map"; }
	virtual ~Map();

private:
	Map(const Map&);
	Map& operator=(const Map&);

protected:
	Map(const string& sName);

	void addExit(Coord*);

	virtual bool parseSpecific(CFileParser*, const string &sEntry);
	virtual bool hasDefinition() const { return true; }
	virtual void factoryCheck(CFileParser*);
};
#endif
