#include "coord.h"
#include <cfileparser.hpp>

Coord::Coord(CFileParser* poParser)
{
	mXTile=poParser->getNextLong("x tile coordinate");
	poParser->getExpectedChar(",");
	mYTile=poParser->getNextLong("y tile coordinate");
}

CrossCoord::CrossCoord(CFileParser* poParser)
: Coord(poParser)
{
	poParser->checkExpectedString("with");
	string sCrossType=poParser->getNextIdentifier("cross type (doord, flag, ship ...)");

	if (sCrossType=="ship")
		miCrossType=SHIP;
	else if (sCrossType=="door")
		miCrossType=DOOR;
	else if (sCrossType=="teleport")
		miCrossType=TELEPORT;
	else if (sCrossType=="flag")
		miCrossType=FLAG;
	else if (sCrossType=="unknown")
	{
		cerr << "Warning : a cross type is defined as 'unknow'." << endl;
	}
	else
		poParser->throw_("Unknown cross type: "+sCrossType);
}

void Coord::setMap(Map* poMap)
{
	mpoWhere=poMap;
}

