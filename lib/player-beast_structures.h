#ifndef PLAYER_BEAST_STRUCTURES
#define PLAYER_BEAST_STRUCTURES

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
    int x;
    int y;
    int isActive;
    char map[5][5];
};

#endif
