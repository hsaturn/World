#include "bot_conf.h"
#include <cfileparser.hpp>
#include "object.h"
#include "map.h"
#include "item.h"
#include "npc.h"

BotConf* BotConf::mpoInstance=0;

BotConf* BotConf::getInstance()
{
	if (mpoInstance==0)
		mpoInstance=new BotConf();

	return mpoInstance;
}

void BotConf::deleteInstance()
{
//	Item::clear();
//	Map::clear();
//	Npc::clear();
	delete mpoInstance;
	mpoInstance=0;
}

BotConf::BotConf()
{
	readConfig( "bot.conf");
}

void BotConf::readConfig(const string sFileName)
{
	cout << "  BotConf::readConfig" << endl;
	mpoParser=new CFileParser(sFileName,true,true);
	while(mpoParser->good())
	{

		//Object* p=0;
		string sClass=mpoParser->getNextIdentifier("Configuration class");
		cout << "  class=" << sClass << endl;
		if (sClass=="item")
		{
			Item::factory(mpoParser);
		}
		else if (sClass=="map")
		{
			Map::factory(mpoParser);
		}
		else
		{
			mpoParser->throw_("Unknown class ["+sClass+"]");
		}
	}
	delete mpoParser;
	cout << "Config read with success (" << sFileName << ")" << endl;
}
