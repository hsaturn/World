/*
 * bot_conf.h
 *
 *  Created on: 13 juil. 2012
 *      Author: hsaturn
 */
#ifndef _BOT_CONF_HPP_
#define _BOT_CONF_HPP_

#include <string>

using namespace std;

class CFileParser;

class BotConf
{
private:
	static BotConf* mpoInstance;
	CFileParser* mpoParser;

public:
	static BotConf* getInstance();
	static void deleteInstance();

private:
	BotConf();
	BotConf(const BotConf&);
	BotConf& operator=(const BotConf&);

	void readConfig(const string sFileName);
};

#endif
