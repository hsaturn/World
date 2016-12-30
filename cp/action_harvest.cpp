#include "action_harvest.h"
#include "item.h"
#include <cfileparser.hpp>

ActionHarvest::ActionHarvest(CFileParser* poParser)
{
	if (poParser->peekChar()=='{')
	{
		parse(poParser,"harvest");
	}
}

bool ActionHarvest::parseSpecific(CFileParser* poParser, const string &sEntry)
{
	if (sEntry=="wear")
	{
		cout << "Wear not implemented" << endl;
		ItemFactory::parse(poParser);
		return true;
	}
	return false;
}

bool ActionHarvest::pushActions(BotQueue& botQueue)
{
	cerr << "NYI BotActionHarvest::pushActions" << endl;
	return false;
}
