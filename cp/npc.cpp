#include "npc.h"
#include "coord.h"
#include "object.cpp"

#include <cfileparser.hpp>

Npc::Npc(const string &sName)
:
Object(sName),
pos(0),
mbStorage(false)
{
}

Npc::~Npc()
{
	if (pos)
	{
		delete pos;
	}

}

bool Npc::parseSpecific(CFileParser* poParser, const string &sEntry)
{
	if (sEntry=="location")
	{
		pos=new Coord(poParser);
	}
	else if (sEntry=="storage")
	{
		setStorage(poParser);
	}
	else
		return false;
	return true;
}

void Npc::postFactory(CFileParser* poParser)
{
	if (pos==0)
		pos=new Coord(poParser);
	if (poParser->tryString("storage",false))	// FIXME true does not work
	{
		setStorage(poParser);
	}
}

void Npc::setStorage(CFileParser* poParser)
{
	if (mbStorage)
				poParser->throw_("Storage set twice for "+getName());
			mbStorage=true;
}
