#ifndef BEAST_FUNC
#define BEAST_FUNC
#include "player-beast_structures.h"
#include "unistd.h"
#include "boardFunc.h"


extern struct beast_t beast[BEAST_SIZE];
extern int actualBeastSize;

void* startBeast(void* arg);
void initBeast();
void generateBeastLocation(struct board_t *map, struct beast_t* beastClient);
//void updateBeast(struct beast_t beasts[]);

void generateRandMove(struct beast_t* beastClient);
void beastMove(struct beast_t* beastClient, int playerX, int playerY);
int checkForPlayer(struct beast_t* beastClient);
int validateBeastMove(int x, int y, struct board_t *map);
int ifBeastInBush(const struct beast_t *beastClient,const struct coordination_t* bushCoord);
void updateBeast(struct beast_t beasts[]);
void updateBeastMap(struct beast_t* beastClient,struct board_t* map);

void eatPlayer(struct beast_t* beastClient ,struct player_t players[]);
void killPlayer(struct player_t* playerClient);
void genePlayerLocation(struct board_t *map, struct player_t* playerClient);

#endif