#ifndef PLAYER_BEAST_STRUCTURES
#define PLAYER_BEAST_STRUCTURES
#include "pthread.h"

#define PLAYERS_SIZE 4
#define BEAST_SIZE 4

enum key_t {
    NONE = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
};
enum clientInBush{
    OUT_BUSH=0,
    IN_BUSH=1
};

struct player_t{
    int PID;
    int ID;
    int clientSocket;
    int isConnected;
    int x;
    int y;
    int coins;
    int coinsInCamp;
    int deaths;
    char map[5][5];
};

struct beast_t{
    int PID;
    int ID;
    int x;
    int y;
    int isActive;
    enum key_t beastMove;
    enum clientInBush beastInBush;
    pthread_mutex_t beastMutex;
    char map[5][5];
};

#endif
