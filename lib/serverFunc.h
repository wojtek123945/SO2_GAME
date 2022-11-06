#ifndef SERVER_FUNC
#define SERVER_FUNC

#include "player-beast_structures.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "ncurses.h"
#include "pthread.h"
#include "semaphore.h"
#include "boardFunc.h"
#include <arpa/inet.h>
#include "playerFunc.h"
#include "beastFunc.h"

#define PLAYERS_SIZE 4
#define MILI_SECOND_PER_ROUND 5

//enum key_t {
//    NONE = 0,
//    UP = 1,
//    DOWN = 2,
//    LEFT = 3,
//    RIGHT = 4
//};

extern enum key_t KeyL[4];
extern enum clientInBush playerInBush[4];
extern struct player_t players[PLAYERS_SIZE];
extern pthread_mutex_t mutex;
extern pthread_t beastThread[4];

extern int ROUND;
extern int MOVECLICK;
extern int serverSocket;
extern struct sockaddr_in serverAddr;
extern int CLIENTS;

int serverCreate(const char *ip, int port, int numberOfClient);

void *connectToServer(void *arg);
void sendMess(struct player_t players[]);
void *recvMess(void *arg);
void setKeyL(int playerMove, int playerId);
void *setClick(void *arg);

void tour();

int serverClick(int moveClick, struct board_t *map);

void generateCoin(struct board_t *map, enum boardObstacles obstacles);

#endif