#ifndef BOARD_FUNC_H
#define BOARD_FUNC_H

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "ncurses.h"
#include "string.h"


#define BOARD_WIDTH 32
#define BOARD_HEIGHT 32
#define BUSH_SIZE 11
#define CAMPSIDE_SIZE 1
#define COIN_BAG_SIZE 16

enum boardObstacles{
    FLOOR=' ',
    WALL='_',
    BUSH='#',
    COIN='c',
    TREASURE='t',
    LARGE_TREASURE='T',
    COIN_BAG='D',
    CAMPSIDE='~',
    PLAYER='1',
    PLAYER_2='2',
    PLAYER_3='3',
    PLAYER_4='4',
    BOT_BEAST='*'
};

struct board_t{
    unsigned char** boardPage;
    size_t width;
    size_t height;
};
struct coordination_t{
    int x;
    int y;
};
struct bag_t{
    struct coordination_t coord;
    int coins;
};
struct coinBag_t{
    int size;
    struct bag_t bag[COIN_BAG_SIZE];
};

extern struct board_t *board;
extern struct coinBag_t coinBag_15;
extern struct coordination_t busheshPlaces[BUSH_SIZE];
extern struct coordination_t campsidePlaces[CAMPSIDE_SIZE];
extern int boardFreeSpace;

void initializeBoard(struct board_t** gameBoard);
void initializeBoardFromFile(struct board_t** gameBoard, char* filename);
void drawBoard(struct board_t* gameBoard);
void drawObject(enum boardObstacles b, int y, int x);
void setColors();
void updatePermamentObj(struct board_t* map);
void drawLegend(int x, int y);

void freeBoard(struct board_t**gameBoard);
#endif