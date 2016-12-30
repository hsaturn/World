
#ifndef _BOT_H
#define _BOT_H

// trublion trubli trubli

#include "../multiplayer.h"
#include "../actors.h"
#include "../pathfinder.h"
#include "../bags.h"
#include "../client_serv.h"
#include "../3d_objects.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int*	latest_bag;
	int*	no_near_actors;
	int*	max_actors;
	int*	harvesting;
	int*	tile_map_size_x;
	int*	tile_map_size_y;
	SDL_mutex*	actors_lists_mutex;
	near_actor* near_actors;
	actor**		actors_list;
	actor*		your_actor;
	TCPsocket	my_socket;
	bag*		bag_list;
	object3d**	objects_list;
} t_bot_data;

typedef struct {
	int (*pf_find_path)			(int x, int y);
	void (*pick_up_all_items)	(void);
	void (*pf_destroy_path)		(void);
	int (*my_tcp_send)			(TCPsocket my_socket, const Uint8 *str, int len);
	void (*open_bag)			(int object_id);

} t_bot_functions;

extern bag bag_list[NUM_BAGS];
const char* protoToStr(const Uint8 *str);
void dumpProto(const Uint8 *str, int iLen);
int openNearestBag();
int botLibParseInput(const char* data,int len);
void botLibChangeMap(const char* pMapName);
int botLibRunCommands(const char* pCommands);
float botDistanceToMe(int x_pos, int y_pos);
void botUnload();
void botReload();

#define init_bot botInit();

#ifdef __cplusplus
} // extern "C"


#endif

#endif
