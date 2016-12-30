#include <bot.h>
#include "../text.h"
#include "../bags.h"
#include "../actors.h"
#include "../e3d.h"
#include <SDL/SDL.h>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
#include "StringUtil.hpp"
#include <fstream>
#include "bot_conf.h"
#include "bot_queue.h"
#include "item.h"
#include "foo.h"
#include <csexception.hpp>

BotConf* poConfig=0;

using namespace std;

extern "C" {

void banner(const string &s, ostream& out)
{
	static string sStars="****************************************";
	int padding=(sStars.length()-s.length())/2;
	if (padding<0)
		padding=0;
	out << sStars << endl;
	out << setw(padding) << s << endl;
	out << sStars << endl;
}

void __attribute__ ((constructor)) my_init()
{
	banner( "BOT CONSTRUCTOR CALLED VER:"+StringUtil::to_string(BUILD) , cerr);
}

void __attribute__ ((destructor)) my_fini()
{
	banner( "END OF BOT !" , cerr);
	cerr << endl;
}
Uint32 botRunCommands(const char* p);

// #define LOG_TO_BOT(color,buffer)	put_colored_text_in_buffer(color,CHAT_SERVER,(const Uint8*)buffer,20) /*!< logs the text in buffer with the specified color to the console. */

#define LOG_TO_BOT(color, buffer) { cout << buffer << endl; }

static bool				botPause=false;
static SDL_TimerID			botTimer=0;
static 	BotQueue			botQueue;
static t_bot_data*			pbot_data;
static t_bot_functions*		pbot_funs;

#define latest_bag 			(*(pbot_data->latest_bag))
#define actors_lists_mutex (pbot_data->actors_lists_mutex)
#define near_actors			(pbot_data->near_actors)
#define no_near_actors		(*(pbot_data->no_near_actors))
#define max_actors			(*(pbot_data->max_actors))
#define actors_list			(pbot_data->actors_list)
#define your_actor			(pbot_data->your_actor)
#define my_socket			(pbot_data->my_socket)
#define bag_list			(pbot_data->bag_list)
#define harvesting			(*(pbot_data)->harvesting)
#define tile_map_size_x		(*(pbot_data)->tile_map_size_x)
#define tile_map_size_y		(*(pbot_data)->tile_map_size_y)
#define objects_list		(pbot_data->objects_list)

#define pf_find_path		(*pbot_funs->pf_find_path)
#define pick_up_all_items	(*pbot_funs->pick_up_all_items)
#define pf_destroy_path		(*pbot_funs->pf_destroy_path)
#define my_tcp_send			(*pbot_funs->my_tcp_send)
#define open_bag			(*pbot_funs->open_bag)

int openLatestBag()
{
	// TODO latest_bag n'est pas remis à -1
	if (latest_bag==-1)
		return 0;

	open_bag(bag_list[latest_bag].obj_3d_id);
	latest_bag=-1;
	return 1;
}

void botChangeMap(const char* pMapName)
{
	latest_bag=-1;
}

Uint32 botSequencer(Uint32 interval, void * data)
{
	Uint32 iNextEvent= 250+(rand() / (RAND_MAX/250));

	if (!botQueue.empty())
	{
		if (botPause==false)
		{
			string s=botQueue.front();
			StringUtil::trim(s);
			if (s.length())
			{
				iNextEvent=botRunCommands(s.c_str());
				if (botQueue.empty())
					cout << "End of queue !" << endl;
			}
			else
			{
				botQueue.pop_front();
				iNextEvent=100;
			}
		}
		else
		{
			cout << "Paused." << endl;
		}
	}
	return iNextEvent;
}

int botBuild()
{
	foo* f=new foo ;
	f->view();
 	return BUILD;
}

void botInit(t_bot_data* pb, t_bot_functions* pf)
{
	cout << "Bot init... " << BUILD << endl;

	try
	{
		cout << "Bot loading configuration..." << endl;
		poConfig=BotConf::getInstance();
		cout << "Bot config loaded" << endl;
	}
	catch(CSException* e)
	{
		cerr << *e << endl;
	}
	pbot_data=pb;
	pbot_funs=pf;
	if (botTimer==0)
	{
		LOG_TO_BOT(c_blue1, "BOT INIT test");
		latest_bag=-1;
	}
	cout << "Bot initialization done !" << endl;
}

void botEnd()
{
	cout << "Bot end..." << BUILD << endl;
	if (botTimer)
	{
		SDL_RemoveTimer(botTimer);
		botTimer=0;
		BotConf::deleteInstance();
	}
}

void botDumpStack()
{
	cout << "--------[ bot stack next first]-------------" << endl;
	BotQueue::const_iterator oit=botQueue.begin();
	while(oit!=botQueue.end())
	{
		cout << *oit << endl;
		oit++;
	}
	cout << "--------[ last action ]--------" << endl;

}

int botParseInput(const char* data,int len)
{
	string s(data+1,len-1);
	static string sLast;

	if (data[0]=='*' && len==1)
	{
		botDumpStack();
		return 1;
	}
	else if (data[0]=='?')
	{
		if (StringUtil::expectWord(s,"near_actors")||StringUtil::expectWord(s,"nna"))
		{
			stringstream out;
			LOG_TO_BOT(c_blue1,StringUtil::to_string(no_near_actors).c_str());
			out << "Near actors : " << no_near_actors << endl;
			out << "Your pos : " << your_actor->x_pos << ':' << your_actor->y_pos << endl;
			for(int i=0; i<no_near_actors; i++)
			{
				//if (near_actors[i].actor)
				{
					actor* pActor=actors_list[near_actors[i].actor];
					out << pActor->actor_type << ' ' << pActor->actor_name << ' ' << pActor->actor_id << ": pos " << pActor->x_pos << "x" << pActor->y_pos
					  << " distance: " << botDistanceToMe(pActor->x_pos, pActor->y_pos);
					  if (pActor->dead)
						  out << " (dead)";
					 out << endl;
				}
			}
			LOG_TO_BOT(c_blue1,out.str().c_str());
		}
		else if (s=="")
		{
			stringstream out;
			out << "Bot queue size : " << botQueue.size();
			LOG_TO_BOT(c_blue1,out.str().c_str());
		}
		else
			return 0;
		return 1;
	}
	else if (data[0]=='!' && len==1)
	{
		s=sLast;
	}
	else if (data[0]!='&')
	{
		botPause=true;
		return 0;
	}
	else if (len==1)
	{
		botPause=false;
	}
	else
	{
		sLast=s;
	}

	if ((s=="stop") || (s=="&"))
	{
		botQueue.clear();
	}
	else if (s=="continue" || s=="cont")
	{
		botPause=false;
	}
	else if (s=="pause")
	{
		botPause=true;
	}
	else if (s=="run")
	{
		botTimer = SDL_AddTimer (1000, botSequencer, NULL);
	}
	else if (s=="stack")
	{
		if (StringUtil::expectWord(s,"stack"))
		{
			botDumpStack();
		}
	}
	else
	{
		botQueue.push_back(s);
	}

	return 1;
}

float botDistanceToMe(int x_pos, int y_pos)
{
	if (your_actor)
	{
		int x=x_pos-your_actor->x_pos;
		int y=y_pos-your_actor->y_pos;
		return sqrt(x*x+y*y);
	}
	else
		return 9999;
}

bool botNear(int x, int y)
{
	return (abs(y-x)<=1);
}

string botMoveToCmd(int x, int y)
{
	return "move "+StringUtil::to_string(x)+" "+StringUtil::to_string(y);
}
string botMoveToActorCmd(actor* pActor)
{
	return botMoveToCmd(pActor->x_tile_pos,pActor->y_tile_pos);
}

/**
 * Retourne un nombre aléatoire mini+rand(delta)
 */
string botRnd(int delta, int base=250)
{
	return StringUtil::to_string(base+(rand() / (RAND_MAX/delta)));
}

int botFind(string &s)
{
	string sFind=StringUtil::getWord(s);
	cout << "Finding " << sFind << endl;
	int iFlags=0;
	int iCount=0;
	bool bFlag=true;
	while(bFlag)
	{
		if (StringUtil::expectWord(s,"harvestable"))
		{
			iFlags|=OBJ_3D_HARVESTABLE;
		}
		else if (StringUtil::expectWord(s,"entrable"))
		{
			iFlags|=OBJ_3D_ENTRABLE;
		}
		else if (StringUtil::expectWord(s,"bag"))
		{
			iFlags|=OBJ_3D_BAG;
		}
		else if (StringUtil::expectWord(s,"mine"))
		{
			iFlags|=OBJ_3D_MINE;
		}
		else
			bFlag=false;
	}
	static map<string,string >	mapConvert;
	if (mapConvert.size()==0)
	{
		mapConvert["redsnapdragons"]="flowerbush2.e3d";
		mapConvert["whitechanterelle"]="mushroom4.e3d";
		mapConvert["tigerlilly"]="flowerorange2.e3d";
		mapConvert["woodlogs"]="log2.e3d";
		mapConvert["chrysanthemum"]="flowerpink1.e3d";
		mapConvert["redcurrents"]="bush_redberry.e3d";
		mapConvert["lilacs"]="flowerbush1.e3d";
	}
	int iFound=-1;
	float fMinDist=9999;

	map<string,string>::const_iterator oit=mapConvert.find(sFind);
	if (oit!=mapConvert.end())
	{
		sFind=oit->second;
	}
	else	// find approx
	{
		string sFound="";
		oit=mapConvert.begin();
		while(oit!=mapConvert.end())
		{
			if (oit->first.find(sFind)!=string::npos)
			{
				cout << "  Candidate : " << oit->first << " " << oit->first.find(sFind) << endl;
				if (sFound=="")
					sFound=oit->second;
				else
				{
					cout << "Too many choices..." << endl;
					return -1;
				}
			}
			oit++;
		}
		if (sFound.length())
			sFind=sFound;
	}

	bool bAny=sFind=="any";
	for(int i=0;i<MAX_OBJ_3D;i++)
	{
		if (objects_list[i]==NULL)
			continue;
		string sFileName=objects_list[i]->file_name;
		if ((bAny || sFileName.find(sFind)!=string::npos) && (iFlags==0 || (objects_list[i]->flags&iFlags)))
		{
			float fDist=botDistanceToMe(objects_list[i]->x_pos,objects_list[i]->y_pos);
			if (fMinDist>fDist)
			{
				iFound=i;
				fMinDist=fDist;
			}
			iCount++;
			cout << "found [" << i << ", flags " << objects_list[i]->flags << " at dist " << fDist << ":" << objects_list[i]->x_pos << "x" << objects_list[i]->y_pos << ":" << objects_list[i]->file_name << endl;
		}
	}
	cout << "Searching " << sFind << " flags " << iFlags << endl;
	if (iCount)
		cout << iCount << " results." << endl;
	if (iFound>=0)
		cout << "best is " << iFound << " at dist " << setprecision(2) << fMinDist << ": " << ((int)(2*objects_list[iFound]->x_pos)) << "," << ((int)(2*objects_list[iFound]->y_pos)) << " " << objects_list[iFound]->file_name << endl;

	else
		cout << "Not found !" << endl;
	return iFound;
}

/**
 * @return Nbre de millisecondes avant next run
 */
Uint32 botRunCommands(const char* p)
{
	int iUnstack=1;
	Uint32 iNextEvent= 250+(rand() / (RAND_MAX/250));

	string s(p);

	cout << SDL_GetTicks() << " cmd(" << botQueue.size() << ") :" << s << endl;
	while(s.length())
	{
		StringUtil::trim(s);

		if (StringUtil::expectWord(s,"bag"))
		{
			pf_destroy_path();
			if (!openLatestBag())
			{
				LOG_TO_BOT(c_red2,"No bag");
			}
		}
		else if (StringUtil::expectWord(s,"then"))
		{
			break;
		}
		else if (StringUtil::expectWord(s,"bga"))
		{
			s=s+" bag getall";
		}
		else if (StringUtil::expectWord(s,"getall"))
		{
			pick_up_all_items();
		}
		else if (StringUtil::expectWord(s,"attack"))
		{
			botQueue.repushFront("attack "+s);
			string sName=StringUtil::getWord(s);
			int iActorType=StringUtil::getLong(sName);
			if (iActorType==0)
			{
				iActorType=-1;
			}
			if (iActorType >1 || sName.length())
			{
				float fMinDist=9999;
				iUnstack=0;
				actor* pFoundActor=0;
				LOCK_ACTORS_LISTS();	//lock it to avoid timing issues
				for(int i=0; i<max_actors; i++)
				{
					if(actors_list[i])
					{
						actor* pActor=actors_list[i];
						if ((pActor->actor_type==iActorType || strcasecmp(pActor->actor_name, sName.c_str())==0 )  && pActor->dead==0)
						{
							float dist=botDistanceToMe(pActor->x_pos, pActor->y_pos);
							if (dist<fMinDist)
							{
								pFoundActor=pActor;
								fMinDist=dist;
							}
						}
					}
				}
				UNLOCK_ACTORS_LISTS();	//lock it to avoid timing issues

				if (pFoundActor)
				{
					stringstream out;
					if (fMinDist>=2)
					{
						cout << "Moving near " << pFoundActor->actor_id << " s=" << botQueue.size() << endl;
						string sAttack=botQueue.front();
						botQueue.repushFront(botMoveToActorCmd(your_actor));	// return near position after attack
						botQueue.push_front(sAttack+" nop");
						botQueue.push_front(botMoveToActorCmd(pFoundActor));
					}
					else
					{
						pf_destroy_path();
						cout << "Attacking " << pFoundActor->actor_id << " dist :"  << fMinDist << endl;
						Uint8 str[10];
						str[0] = ATTACK_SOMEONE;
						*((int *)(str+1)) = SDL_SwapLE32((int)pFoundActor->actor_id);
						my_tcp_send (my_socket, str, 5);
						latest_bag=-1;

						botQueue.repushFront(s);		// Remove attack xxx and push next actions (if there was some)
						s="";						// and keep it for after attack

						// TODO Wait dead id

						botQueue.push_front("wait bag 20000 wait 1000 bag wait 500 getall");
					}
				}
				else
				{
					LOG_TO_BOT(c_red1,"No target found waiting ...");
				}
			}
			else
			{
				LOG_TO_BOT(c_red1,(StringUtil::to_string("Bad attack actor_type : ")+StringUtil::to_string(iActorType)).c_str());
			}
		}
		else if (StringUtil::expectWord(s,"wait"))
		{
			static Uint32 iRelease;
			static bool bWaiting=false;
			long iLong=0;
			string sRepush;
			string sCmd;

			// cerr << endl << "wait2-1 " << s << " s=" << botQueue.size() << endl;

			// Supprimer les commandes avant le wait
			botQueue.repushFront("wait "+s);

			sCmd=StringUtil::getWord(s);
			iLong=StringUtil::getLong(s);
			if (iLong==0)
			{
				iLong=StringUtil::getLong(sCmd);
			}
			// cerr << "wait2-3 " << s << " l=" << iLong << " sCmd=" << sCmd <<   endl;


			if (!bWaiting)
			{
				iRelease=SDL_GetTicks()+iLong;
				cout << "  Start wait till " << iRelease;
				bWaiting=true;
			}
			else if (bWaiting && SDL_GetTicks()>iRelease)
			{
				cout << "  End waiting " << endl;
				bWaiting=false;
				iUnstack=0;
				botQueue.repushFront(s);
				break;
			}

			if (sCmd == "bag")
			{
				if (latest_bag!=-1)
				{
					iUnstack=0;
					botQueue.repushFront(s);
					bWaiting=false;
					break;
				}
			}
			else if (!bWaiting)
			{
				sRepush=s;
			}

			// cerr << "wait2-4 " << s << endl;

			if (sRepush.length())
			{
				botQueue.push_front(sRepush);
				iUnstack=0;
				// cerr << "repushing " << sRepush << endl;
			}
			else
			{
				botQueue.push_front("nop");
				iUnstack=1;
			}
			s="";
			break;
		}
		else if (StringUtil::expectWord(s,"nop"))
		{
		}
		else if (StringUtil::expectWord(s,"harvest"))
		{
			int iId=StringUtil::getLong(s);
			if (iId==0)
			{
				s=s+" harvestable";
				int iFound=botFind(s);
				if (iFound>=0)
				{
					botQueue.repushFront(s);
					botQueue.push_front(string("harvest ")+StringUtil::to_string(objects_list[iFound]->id));
					botQueue.push_front("wait 3000");
					botQueue.push_front(botMoveToCmd(objects_list[iFound]->x_pos*2,objects_list[iFound]->y_pos*2));
					botDumpStack();
					iUnstack=0;
				}
			}
			else
			{
				Uint8 str[10];
				str[0] = HARVEST;
				*((Uint16 *)(str+1)) = SDL_SwapLE16((Uint16)iId);
				my_tcp_send (my_socket, str, 3);
			}
		}
		else if (StringUtil::expectWord(s,"repeat"))
		{
			int iRepeat=StringUtil::getLong(s);

			if (iRepeat)
			{
				iRepeat--;
				botQueue.repushFront(string("repeat "+StringUtil::to_string(iRepeat)+" "+s));
				botQueue.push_front(s);
			}
		}
		else if (StringUtil::expectWord(s,"get"))
		{
			int iCount=StringUtil::getLong(s);
			string sName=StringUtil::getWord(s);

			Item::getSome (sName,iCount, botQueue);
		}
		else if (StringUtil::expectWord(s,"desc"))
		{
			int iId=StringUtil::getLong(s);
			for(int i=0;i<MAX_OBJ_3D;i++)
			{
				if (objects_list[i]==NULL)
					continue;
				if (objects_list[i]->id==iId)
				{
					cout << "found at " << objects_list[i]->x_pos << "x" << objects_list[i]->y_pos << ":" << objects_list[i]->file_name << endl;
				}
			}
		}
		else if (StringUtil::expectWord(s,"find"))
		{
			botFind(s);
		}
		else if (StringUtil::expectWord(s,"move"))
		{
			iUnstack=0;

			// Disable previous commands
			botQueue.repushFront("move "+s);

			int x=StringUtil::getLong(s);
			int y=StringUtil::getLong(s);

			// Skip move if further moves
			if (botQueue.size()>1 && (botQueue[1].substr(0,5)=="move "))
			{
				cout << "move unstacked !";
				iUnstack=1;
				continue;
			}

			static int iLastX=-1;	// Don't find to many paths
			static int iLastY=-1;
			static Uint32 iDurationEnd;
			Uint32 iDuration;

			if (StringUtil::expectWord(s,"duration"))
			{
				iDuration=StringUtil::getLong(s);
			}
			else
			{
				iDuration=20000;
			}

			if (botNear(your_actor->x_tile_pos, x) && botNear(your_actor->y_tile_pos,y))
			{
				// destination reached !
				botQueue.repushFront(s);
				break;
			}

			if (botNear(iLastX,x) && botNear(iLastY,y))
			{
				if (your_actor->moving)
				{
					if (SDL_GetTicks()>iDurationEnd)
					{
						botQueue.repushFront(s);	// All more action after duration
					}
					// Skipping move dest (near or same as previous one)
					break;
				}
			}

			cout << "moving to " << x << "x" << y << ' ';

			if (pf_find_path(x,y)
					|| pf_find_path(x-1,y) || pf_find_path(x+1,y)
					|| pf_find_path(x,y-1)  || pf_find_path(x,y+1)
					|| pf_find_path(x-1,y-1) || pf_find_path(x+1,y-1)
					|| pf_find_path(x-1,y+1) || pf_find_path(x+1,y+1)
					|| pf_find_path(x-2,y) || pf_find_path(x+2,y)
					|| pf_find_path(x,y-2) || pf_find_path(x,y+2)
					|| pf_find_path(x-2,y+1) || pf_find_path(x+2,y+1)
					|| pf_find_path(x-2,y-1) || pf_find_path(x+2,y-1)
					|| pf_find_path(x-1,y-2) || pf_find_path(x-1,y+2)
					|| pf_find_path(x+1,y-2) || pf_find_path(x+1,y+2)
					|| pf_find_path(x-2,y-2) || pf_find_path(x-2,y+2)
					|| pf_find_path(x+2,y-2) || pf_find_path(x+2,y+2)
					)
			{
				iLastX=x;
				iLastY=y;
				iDurationEnd=SDL_GetTicks()+iDuration;
				iNextEvent=iDuration;
				cout << "ok ! s=" << botQueue.size();
				// botQueue.repushFront(s);
			}
			else
			{
				cout << " :-(   find_path returned 0" << endl;
				iUnstack=1;
				botDumpStack();
			}
			s="";
			cout << endl;
		}
		else
		{
			LOG_TO_BOT(c_red1,string("Unknown command : ")+s);
			break;
		}
	}

	if (iUnstack &&  botQueue.size())
	{
		botQueue.pop_front();
	}

	return iNextEvent;
}

}



