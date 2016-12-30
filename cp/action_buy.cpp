#include "action_buy.h"
#include <cfileparser.hpp>

ActionBuy::ActionBuy(CFileParser* poParser)
{
	Action::parse(poParser,"buy");
}

ActionBuy::~ActionBuy()
{

}

bool ActionBuy::parseSpecific(CFileParser* poParser, const string &sEntry)
{
	bool bRet=true;
	if (sEntry=="pcb")
	{
		miPcb=poParser->getNextLong("pcb value");
	}
	else
	{
		poParser->readNextRow();
		// FIXME all is allowed in buy action
	}
	return bRet;
}

bool ActionBuy::pushActions(BotQueue &botQueue)
{
	cerr << "ActionBuy::pushActions not implemented";
	return true;
}
