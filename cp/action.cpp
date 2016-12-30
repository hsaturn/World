#include "action.h"
#include "item.h"
#include <cfileparser.hpp>

void Action::parse(CFileParser* poParser, const string& sAction)
{
	poParser->getExpectedChar("{");
	while(poParser->good())
	{
		string sEntry=poParser->getNextIdentifier("mix entry");
		if (sEntry =="item")
		{
			lstItems.push_front(
				new ActionItem(
					Item::factory(poParser),
					poParser->getNextLong("count")));
		}
		else if (parseSpecific(poParser,sEntry)==false)
		{
			poParser->throw_("Unknown "+sAction+" entry ["+sEntry+"]");
		}

		if (poParser->tryChar('}'))
		{
			break;
		}
	}
}

Action::~Action()
{
	for(iterator oit=begin(); oit!=end(); oit++)
	{
		delete *oit;
	}
}
