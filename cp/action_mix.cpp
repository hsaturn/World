#include "action_mix.h"
#include "item.h"
#include <cfileparser.hpp>
#include <iostream>

using namespace std;

ActionMix::~ActionMix()
{

}

ActionMix::ActionMix(CFileParser* poParser)
{
	Action::parse(poParser,"mix");
}

bool ActionMix::parseSpecific(CFileParser* poParser, const string &sEntry)
{
	bool bRet=true;
	if (sEntry=="food")
	{
		msFoodRequired=poParser->getNextLong("amount of food required");
	}
	else
		bRet=false;
	return bRet;
}

bool ActionMix::pushActions(BotQueue &botQueue)
{
	cerr << "ActionMix::pushActions" << endl;

	for(const_iterator oit=begin(); oit!=end(); oit++)
	{
		cout << "get item " << (*oit)->getItem()->getName() << " x" << (*oit)->getCount() << endl;
	}
	return 0;
}
