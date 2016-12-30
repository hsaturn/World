#ifndef _BOT_COORD_H_
#define _BOT_COORD_H_

class CFileParser;
class Map;

class Coord
{
private:
	Map* mpoWhere;	// CAN BE 0 !!!
	int mXTile;
	int mYTile;

public:
	Coord(CFileParser*);

	int getXTile() const { return mXTile; }
	int getYTile() const { return mYTile; }

	// In addition, a coord can be set to a specific map
	void setMap(Map*);

	/**
	 * /!\ CAN RETURN NULL POINTER (No map affected)
	 */
	Map* getMap() { return mpoWhere; }

};

class CrossCoord : public Coord
{
public:
	typedef enum
	{ DOOR,SHIP,FLAG,TELEPORT } CrossType;

private:
	CrossType	miCrossType;

public:
	CrossCoord(CFileParser*);
	CrossType getCrossType() const { return miCrossType; }

};

#endif
