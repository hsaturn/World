#include "item.h"
#include "object.cpp"

#include <cfileparser.hpp>


Item::Item(const string& sName)
:
Object(sName)
{
}

Item::~Item()
{
	if (mpoGet)
		delete mpoGet;
}

void Item::factoryCheck(CFileParser* poParser)
{
	if (miEmus==0)
	{
		miEmus=1;
		cerr << "Warning item " << getName() << " default emus set to 1" << endl;
	}
}

bool Item::parseSpecific(CFileParser* poParser, const string &sEntry)
{
	if (sEntry=="get")
	{
		mpoGet=new Get(poParser);
	}
	else if (sEntry=="emu")
	{
		miEmus=poParser->getNextLong("weight value");
	}
	else
		return false;

	return true;
}

bool Item::getSome(const string& sName, int iCount, BotQueue& botQueue)
{
	bool bRet=false;
	{
		cerr << "No item " << sName << " found." << endl;
	}
	return bRet;
}

bool Item::pushActions(BotQueue &botQueue)
{
	bool bRet=true;
	if (mpoGet)
	{
		mpoGet->pushActions(botQueue);
	}
	else
	{
		bRet=false;
		cerr << "Item " << getName() << " has no valid get method." << endl;
	}
	return bRet;
}
