#include "map.h"
#include "coord.h"
#include "npc.h"
#include "item.h"
#include "object.cpp"


#include <cfileparser.hpp>

// Map::TContainer Map::mapMaps;

Map* poParent=0;	// FIXME Bad design

Map::Map(const string& sName)
:
Object(sName),
mpoParentMap(poParent),
mbEntrySet(false)
{
	// cout << "Creating map " << sName << endl;
	poParent=0;
}


Map::~Map()
{
	for(item_iterator oit=mapItems.begin(); oit!=mapItems.end(); oit++)
	{
		for(list<Coord*>::iterator oitC=oit->second.begin(); oitC!=oit->second.end(); oitC++)
		{
			delete *oitC;
		}
	}

	for(exit_iterator oit=lstExits.begin(); oit!=lstExits.end(); oit++)
	{
		delete *oit;
	}
}

bool Map::parseSpecific(CFileParser* poParser, const string &sEntry)
{
	if (sEntry=="npc")
	{
		Npc* poNpc=Npc::factory(poParser);
		mapNpcs[poNpc->getName()]=poNpc;
		poNpc->getCoords()->setMap(this);
	}
	else if (sEntry =="exit")
	{
		poParser->readNextRow();
		cerr << "Map::exit not yenort implemented" << endl;
	}
	else if (sEntry=="item")
	{
		Item* poItem=Item::factory(poParser);
		Coord* mpCoord=new Coord(poParser);
		mapItems[poItem->getName()].push_back(mpCoord);
	}
	else if (sEntry=="map")
	{
		poParent=this;
		Map* poMap=Map::factory(poParser);
		poMap->mpoParentMap=this;
	}
	else if (sEntry=="exit")
	{
		Map* poWhere=0;
		if (poParser->checkExpectedString("at",true,false))
		{
			if (mpoParentMap==0)
			{
				poParser->throw_("Map name must be set after exit for not insides maps");
			}
			poWhere=mpoParentMap;
		}
		else
		{
			poWhere=Map::factory(poParser);
		}
		if (poWhere==0)
		{
			poParser->throw_("Should not arrive here (internal error map)");
		}
		CrossCoord* pos=new CrossCoord(poParser);
		pos->setMap(poWhere);
		addExit(pos);
	}
	else if (sEntry=="entry")
	{
		if (mpoParentMap==0)
			poParser->throw_("entry can be used only within a nested map");

		mbEntrySet=true;

		CrossCoord* pos=new CrossCoord(poParser);

		if (pos->getXTile()==0 || pos->getYTile()==0)
		{
			cerr << "Warning : map cross coord badly defined" << endl;
		}
		pos->setMap(this);

		mpoParentMap->addExit(pos);
	}
	else
		return false;

	return true;
}

void Map::factoryCheck(CFileParser* poParser)
{
	if (mpoParentMap && mbEntrySet==false)	// Check if entry has been set for nested map
	{
		poParser->throw_("entry must be set for the nested map "+getName()+" (in map "+mpoParentMap->getName()+")");
	}
	poParent=0;	// FIXME bad design
}

void Map::addExit(Coord *pos)
{
	if (pos->getMap()==0)
	{
		cerr << "Error: where added with no destination map" << endl;
	}
	else
		lstExits.push_back(pos);
}
