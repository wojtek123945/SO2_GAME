#ifndef PLAYER_FUNC_H
#define PLAYER_FUNC_H
#include "player-beast_structures.h"
#include "boardFunc.h"
#include "serverFunc.h"

void initializePlayer(struct player_t *player, struct board_t *board);
void initMapToPlayers(struct player_t player[], const struct board_t *map);

void updatePlayerCoins(struct player_t* player, int obj);
void updatePlayer(struct player_t player[], const struct board_t *map);

void playerColison(struct player_t* p_1, struct player_t* p_2, struct board_t* map);
int validatePlayerMove(int x, int y, struct board_t *map);
void closePlayer(struct player_t *player, struct board_t* map);
int ifPlayerInBush(const struct player_t *player,const struct coordination_t* bushCoord);

void drawPlayerInfo(int x, int y, struct player_t player[], int maxSizePlayer);
#endif