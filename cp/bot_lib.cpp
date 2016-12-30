#include <bot.h>
#include "../text.h"
#include "../bags.h"
#include "../actors.h"
#include <dlfcn.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "../counters.h"
#include "link.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "StringUtil.hpp"

using namespace std;

// Donné par le makefile
#ifdef LIB_FULL_NAME
	#define stringnify_argument(s) stringnify(s)
	#define stringnify(s) #s
	#define BOTLIB_FULL_NAME stringnify_argument(LIB_FULL_NAME)
#else
	#error Missing compilation option -DLIB_FULL_NAME=(full libbot.so path)
#endif

extern "C" {



static void botLibEnd();

void dumpProto(const Uint8 *str, int iLen)
{
	stringstream s;

	s << "TCP " << protoToStr(str) << ' ';
	for(int i=1;i<iLen;i++)
	{
		s << (int)str[i] << ' ';
	}
	string st(s.str());
	cout << st << endl;
}


const char* protoToStr(const Uint8 *str)
{
	static char* p=(char*)malloc(50);
	if (str[0]==MOVE_TO) return "MOVE_TO";
	if (str[0]==SEND_PM) return "SEND_PM";
	if (str[0]==GET_PLAYER_INFO) return "GET_PLAYER_INFO";
	if (str[0]==RUN_TO) return "RUN_TO";
	if (str[0]==SIT_DOWN) return "SIT_DOWN";
	if (str[0]==SEND_ME_MY_ACTORS) return "SEND_ME_MY_ACTORS";
	if (str[0]==SEND_OPENING_SCREEN) return "SEND_OPENING_SCREEN";
	if (str[0]==SEND_VERSION) return "SEND_VERSION";
	if (str[0]==TURN_LEFT) return "TURN_LEFT";
	if (str[0]==TURN_RIGHT) return "TURN_RIGHT";
	if (str[0]==PING) return "PING";
	if (str[0]==HEART_BEAT) return "HEART_BEAT";
	if (str[0]==LOCATE_ME) return "LOCATE_ME";
	if (str[0]==USE_MAP_OBJECT) return "USE_MAP_OBJECT";
	if (str[0]==SEND_MY_STATS) return "SEND_MY_STATS";
	if (str[0]==SEND_MY_INVENTORY) return "SEND_MY_INVENTORY";
	if (str[0]==LOOK_AT_INVENTORY_ITEM) return "LOOK_AT_INVENTORY_ITEM";
	if (str[0]==MOVE_INVENTORY_ITEM) return "MOVE_INVENTORY_ITEM";
	if (str[0]==HARVEST) return "HARVEST";
	if (str[0]==DROP_ITEM) return "DROP_ITEM";
	if (str[0]==PICK_UP_ITEM) return "PICK_UP_ITEM";
	if (str[0]==LOOK_AT_GROUND_ITEM) return "LOOK_AT_GROUND_ITEM";
	if (str[0]==INSPECT_BAG) return "INSPECT_BAG";
	if (str[0]==S_CLOSE_BAG) return "S_CLOSE_BAG";
	if (str[0]==LOOK_AT_MAP_OBJECT) return "LOOK_AT_MAP_OBJECT";
	if (str[0]==TOUCH_PLAYER) return "TOUCH_PLAYER";
	if (str[0]==RESPOND_TO_NPC) return "RESPOND_TO_NPC";
	if (str[0]==MANUFACTURE_THIS) return "MANUFACTURE_THIS";
	if (str[0]==USE_INVENTORY_ITEM) return "USE_INVENTORY_ITEM";
	if (str[0]==TRADE_WITH) return "TRADE_WITH";
	if (str[0]==ACCEPT_TRADE) return "ACCEPT_TRADE";
	if (str[0]==REJECT_TRADE) return "REJECT_TRADE";
	if (str[0]==EXIT_TRADE) return "EXIT_TRADE";
	if (str[0]==PUT_OBJECT_ON_TRADE) return "PUT_OBJECT_ON_TRADE";
	if (str[0]==REMOVE_OBJECT_FROM_TRADE) return "REMOVE_OBJECT_FROM_TRADE";
	if (str[0]==LOOK_AT_TRADE_ITEM) return "LOOK_AT_TRADE_ITEM";
	if (str[0]==CAST_SPELL) return "CAST_SPELL";
	if (str[0]==ATTACK_SOMEONE) return "ATTACK_SOMEONE";
	if (str[0]==GET_KNOWLEDGE_INFO) return "GET_KNOWLEDGE_INFO";
	if (str[0]==ITEM_ON_ITEM) return "ITEM_ON_ITEM";
	if (str[0]==SEND_BOOK) return "SEND_BOOK";
	if (str[0]==GET_STORAGE_CATEGORY) return "GET_STORAGE_CATEGORY";
	if (str[0]==DEPOSITE_ITEM) return "DEPOSITE_ITEM";
	if (str[0]==WITHDRAW_ITEM) return "WITHDRAW_ITEM";
	if (str[0]==LOOK_AT_STORAGE_ITEM) return "LOOK_AT_STORAGE_ITEM";
	if (str[0]==SPELL_NAME) return "SPELL_NAME";
	if (str[0]==SEND_VIDEO_INFO) return "SEND_VIDEO_INFO";
	if (str[0]==POPUP_REPLY) return "POPUP_REPLY";
	if (str[0]==FIRE_MISSILE_AT_OBJECT) return "FIRE_MISSILE_AT_OBJECT";
	sprintf(p,"UNKNOWN (%d)",(int)str[0]);
	return p;
}

void (*fnbotInit)(t_bot_data*, t_bot_functions*)=0;
void (*fnbotEnd)()=0;
int (*fnbotParseInput)(const char* data,int len)=0;
void (*fnbotChangeMap)(const char* pMapName)=0;

void botLibCheck(const char* sFn)
{
	char* error;
	if ((error=dlerror())!=NULL)
	{
		cout <<"Unable to find " << sFn << " : " << error << endl;
	}
	else
		cout << "Function " << sFn << " loaded." << endl;
}
static t_bot_data bot_data;
static t_bot_functions bot_funs;

static void* lib_bot=0;

void botLibUpdateData()
{
	static bool bOnce=true;

	if (bOnce)
	{
		cout << "Setting bot_funs communication struct with bot !" << endl;
		bOnce=false;
		bot_funs.pf_find_path		= pf_find_path;
		bot_funs.pick_up_all_items	= pick_up_all_items;
		bot_funs.pf_destroy_path	= pf_destroy_path;
		bot_funs.my_tcp_send		= my_tcp_send;
		bot_funs.open_bag			= open_bag;

		bot_data.harvesting			= & harvesting;
		bot_data.tile_map_size_x	= & tile_map_size_x;
		bot_data.tile_map_size_y	= & tile_map_size_y;
	}
	bot_data.latest_bag			= &	latest_bag;
	bot_data.actors_lists_mutex	=	actors_lists_mutex;
	bot_data.no_near_actors		= &	no_near_actors;
	bot_data.near_actors		=	near_actors;
	bot_data.max_actors			= &	max_actors;
	bot_data.actors_list		=	actors_list;
	bot_data.your_actor			=	your_actor;
	bot_data.my_socket			=	my_socket;
	bot_data.bag_list			=	bag_list;
	bot_data.objects_list		=	objects_list;


}

static int
callback(struct dl_phdr_info *info, size_t size, void *data)
{
	if(strstr(info->dlpi_name,"libbot"))
	{
		printf("name=%s (%d segments)\n", info->dlpi_name,
				info->dlpi_phnum);

   /*
         int j;
     for (j = 0; j < info->dlpi_phnum; j++)
         printf("\t\t header %2d: address=%10p\n", j,
             (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));*/
	}
    return 0;
}

void botUnload()
{
	if (lib_bot)
	{
		cout << "Unloading bot... " << endl;
		botLibEnd();
		if (dlclose(lib_bot))
		{
			cerr << "ERROR WHILE UNLOADING ..." << dlerror() << endl;
		}
		dl_iterate_phdr(callback, NULL);
		if (dlopen(BOTLIB_FULL_NAME, RTLD_NOLOAD))
		{
			cerr << "ERROR *** Library is still loaded !" << endl;
		}
		lib_bot=0;
		cout << endl;
	}
	fnbotInit=0;
	fnbotEnd=0;
	fnbotChangeMap=0;
	fnbotParseInput=0;
}

void botReload()
{
	botLibUpdateData();
	botUnload();
	cout << "Loading bot" << endl;

	if (dlopen(BOTLIB_FULL_NAME, RTLD_NOLOAD)==NULL)
	{
		cout << "Library is NOT loaded yet, loading it." << endl;
	}
	else
	{
		cout <<  "ERROR : LIBRARY IS ALREADY LOADED !!!!" << endl;
	}

	lib_bot=dlopen(BOTLIB_FULL_NAME, RTLD_NOW);
	if (!lib_bot)
	{
		cout << "ERROR: Bot init error: " << lib_bot << ' ' << dlerror() << endl;
	}
	else if (true)
	{
		cout << "Fake loading..." << endl;
		int (*getBotBuild)()=(int (*)())dlsym(lib_bot, "botBuild");
		cout << "Fake j'ai eu mon symbole:" << (int) getBotBuild << endl;
		if (getBotBuild)
		{
			cout << "Loaded, BUILD=" << ((*getBotBuild)()) << endl;
			cerr << "Loaded, BUILD=" << ((*getBotBuild)()) << endl;
		}
		else
			cout << "???" << endl;
		botUnload();
	}
	else
	{
		cout << "Bot library loaded." << lib_bot  << endl;

		fnbotInit=(void (*)(t_bot_data*, t_bot_functions*))dlsym(lib_bot, "botInit");
		botLibCheck("botInit");

		fnbotEnd=(void (*)())dlsym(lib_bot, "botEnd");
		botLibCheck("fnbotEnd");

		fnbotParseInput=(int (*)(const char* data,int len))dlsym(lib_bot, "botParseInput");
		botLibCheck("fnbotParseInput");

		fnbotChangeMap=(void (*)(const char* data))dlsym(lib_bot, "botChangeMap");
		botLibCheck("fnbotChangeMap");

		cout << "RUNNING INIT ON LOADED BOT" << endl;
		(*fnbotInit)(&bot_data, &bot_funs);
		cout << "DONE :-)" << endl;
	}
}


static void botLibEnd()
{
	botLibUpdateData();
	if (lib_bot && fnbotEnd) (*fnbotEnd)();
}


int botLibParseInput(const char* data,int len)
{
	botLibUpdateData();
	if (strcmp(data,"&reload")==0)
	{
		botReload();
		return 1;
	}
	if (lib_bot)
	{
		cout << ": ";
		if (fnbotParseInput)
		{
			cout << "ok !" << endl;
			return (*fnbotParseInput)(data,len);
		}
		else
		{
			cout << "No fnbotParseInput !" << endl;
		}
	}
	else
		cout << "? ";
	cout << "parse_input (" << data << '/' << len << ')';
	if (data[0]=='&')
		return 1;
	cout << "return 0 [0]=" << data[0] << endl;
	return 0;
}

void botLibChangeMap(const char* pMapName)
{
	botLibUpdateData();
	cout << "change_map: ";
	if (fnbotChangeMap) (*fnbotChangeMap)(pMapName);
	latest_bag=-1;
}

}

