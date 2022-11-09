#ifndef CLIENT_FUNC_H
#define CLIENT_FUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <ncurses.h>
#include <arpa/inet.h>
#include "pthread.h"

struct player{
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
enum boardObstacles {
    FLOOR = ' ',
    WALL = '_',
    BUSH = '#',
    COIN = 'c',
    TREASURE = 't',
    LARGE_TREASURE = 'T',
    COIN_BAG='D',
    CAMPSIDE = '~',
    PLAYER = '1',
    PLAYER_2 = '2',
    PLAYER_3 = '3',
    PLAYER_4 = '4',
    BOT_BEAST = '*'
};

extern int moveClick;
extern int sock;
extern int lostConnection;
extern pthread_mutex_t mutex, waitMutex;
void setColors();
void drawBoard(const struct player *player);
void drawPlayerInfo(int x, int y, struct player* players);

void closePlayer(struct player *player);

void sendMess(const struct player *players);

void* rMess(void* arg);

void drawLegend(int x, int y);

void *setClick(void *arg);


#endif