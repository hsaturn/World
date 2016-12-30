#include "get.h"
#include "action_harvest.h"
#include "action_mix.h"
#include "action_buy.h"

#include <cfileparser.hpp>

Get::Get(CFileParser* poParser)
{
	if (poParser->good())
	{
		string sGet=poParser->getNextIdentifier("get type");
		if (sGet=="harvest")
		{
			moAction=new ActionHarvest(poParser);
		}
		else if (sGet=="mix")
		{
			moAction=new ActionMix(poParser);
		}
		else if (sGet=="buy")
		{
			moAction=new ActionBuy(poParser);
		}
		else
		{
			poParser->throw_("Unknown get action ["+sGet+"]");
		}
	}
}

Get::~Get()
{
	if (moAction)
		delete moAction;
}

void Get::pushActions(BotQueue &botQueue)
{
	if (moAction)
		moAction->pushActions(botQueue);
}

