#include "boardFunc.h"

struct board_t *board;
struct coinBag_t coinBag_15;
struct coordination_t busheshPlaces[BUSH_SIZE];
struct coordination_t campsidePlaces[CAMPSIDE_SIZE];
int boardFreeSpace;

void initializeBoard(struct board_t** gameBoard){
    if(!*gameBoard){
        *gameBoard = malloc(sizeof(struct board_t));
        if(!*gameBoard)
            return;
    }
    struct board_t* newGameBoard = *gameBoard;

    newGameBoard->boardPage = calloc(sizeof(unsigned char*), BOARD_HEIGHT);
    if(!newGameBoard->boardPage)
        return;
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        *(newGameBoard->boardPage + i) = (char*) calloc(sizeof(unsigned char), BOARD_WIDTH);
        if(!*(newGameBoard->boardPage + i)){
            for (int j = 0; j < i; ++j) {
                free(*(newGameBoard->boardPage + i));
            }
            free(newGameBoard->boardPage);
            return;
        }
    }

    newGameBoard->height = BOARD_HEIGHT;
    newGameBoard->width = BOARD_WIDTH;
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        for (int j = 0; j < BOARD_HEIGHT; ++j) {
            if((i == 0 || j == 0) || (i == BOARD_WIDTH-1 || j == BOARD_HEIGHT-1)) {
                *(*(newGameBoard->boardPage + i) + j) = '1';
                continue;
            }
            *(*(newGameBoard->boardPage + i) + j) = '0';
        }
    }
}
void initializeBoardFromFile(struct board_t** gameBoard, char* filename){
    if(!*gameBoard){
        *gameBoard = malloc(sizeof(struct board_t));
        if(!*gameBoard)
            return;
    }
    struct board_t* newGameBoard = *gameBoard;

    FILE* file = fopen(filename, "r");
    if(!file){
        free(newGameBoard);
        return;
    }
    int width;
    int height;
    int error = (int)fscanf(file, "%dx%d%*[\n]c", &width, &height);
    if(error != 2){
        printf("ERROR");
        return;
    }

    newGameBoard->boardPage = calloc(sizeof(unsigned char*), height);
    if(!newGameBoard->boardPage)
        return;

    for (int i = 0; i < height; ++i) {
        *(newGameBoard->boardPage + i) = (char*) calloc(sizeof(unsigned char), width);
        if(!*(newGameBoard->boardPage + i)){
            for (int j = 0; j < i; ++j) {
                free(*(newGameBoard->boardPage + i));
            }
            free(newGameBoard->boardPage);
            return;
        }
    }
    newGameBoard->height = height;
    newGameBoard->width = width;

    int actualHeight=0;
    int bushesTemp = 0;
    int campsideTemp = 0;
    while (1){
        char* text = (char*) malloc(sizeof(char)*(width + 1));
        if(!text){
            return;
        }
        if(feof(file)){
            return;
        }
        error = (int)fscanf(file, "%[^\n]s", text);
        if(error!= 1){
            return;
        }
        fscanf(file,"%*[\n]c");

        for (int i = 0; i < width; ++i) {
            *(*(newGameBoard->boardPage + actualHeight) + i) = *(text + i);
            if(*(*(newGameBoard->boardPage + actualHeight) + i) == '#'){
                busheshPlaces[bushesTemp].y = actualHeight;
                busheshPlaces[bushesTemp].x = i;
                bushesTemp++;
            }else if(*(*(newGameBoard->boardPage + actualHeight) + i) == '~'){
                campsidePlaces[campsideTemp].y = actualHeight;
                campsidePlaces[campsideTemp].x = i;
                campsideTemp++;
            }
        }
        actualHeight++;
        free(text);
    }
}
void setColors(){
    initscr();

    curs_set(false);
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(FLOOR, COLOR_WHITE, COLOR_WHITE);
    init_pair(WALL, COLOR_RED, COLOR_BLACK);
    init_pair(BUSH, COLOR_GREEN, COLOR_WHITE);
    init_pair(COIN, COLOR_BLUE, COLOR_YELLOW);
    init_pair(TREASURE, COLOR_BLUE, COLOR_YELLOW);
    init_pair(LARGE_TREASURE, COLOR_BLACK, COLOR_YELLOW);
    init_pair(COIN_BAG, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CAMPSIDE, COLOR_BLUE, COLOR_CYAN);
    init_pair(PLAYER, COLOR_RED, COLOR_CYAN);
    init_pair(PLAYER_2, COLOR_RED, COLOR_CYAN);
    init_pair(PLAYER_3, COLOR_RED, COLOR_CYAN);
    init_pair(PLAYER_4, COLOR_RED, COLOR_CYAN);
    init_pair(BOT_BEAST, COLOR_BLUE, COLOR_RED);
}
void drawObject(enum boardObstacles color_pair, int x, int y){
    attron(COLOR_PAIR(color_pair));
    switch (color_pair) {
        case FLOOR:
            mvprintw(x,y, " ");
            break;
        case WALL:
            mvprintw(x,y, "_");
            break;
        case BUSH:
            mvprintw(x,y, "#");
            break;
        case COIN:
            mvprintw(x,y, "c");
            break;
        case TREASURE:
            mvprintw(x,y, "t");
            break;
        case LARGE_TREASURE:
            mvprintw(x,y, "T");
            break;
        case COIN_BAG:
            mvprintw(x,y, "D");
            break;
        case CAMPSIDE:
            mvprintw(x,y, "~");
            break;
        case PLAYER:
            mvprintw(x,y, "1");
            break;
        case PLAYER_2:
            mvprintw(x,y, "2");
            break;
        case PLAYER_3:
            mvprintw(x,y, "3");
            break;
        case PLAYER_4:
            mvprintw(x,y, "4");
            break;
        case BOT_BEAST:
            mvprintw(x,y, "*");
            break;
    }
    attroff(COLOR_PAIR(color_pair));
}
void drawBoard(struct board_t* gameBoard){
    if(!gameBoard)
        return;

    updatePermamentObj(gameBoard);
    boardFreeSpace=0;
    for (int i = 0; i < gameBoard->height; ++i) {
        for (int j = 0; j < gameBoard->width; ++j) {
            int x=i, y=j;
            drawObject(*(*(gameBoard->boardPage + i) + j), x, y);
            if(*(*(gameBoard->boardPage + i) + j) == ' ')
                boardFreeSpace++;
        }
    }
    refresh();
}
void freeBoard(struct board_t**gameBoard){
    if(!gameBoard || !*gameBoard)
        return;
    struct board_t* gBoard = *gameBoard;
    for (int i = 0; i < gBoard->height; ++i) {
        if(*(gBoard->boardPage + i))
            free(*(gBoard->boardPage + i));
    }
    free(gBoard->boardPage);
}
void updatePermamentObj(struct board_t* map){
    if(!map)
        return;
    for (int i = 0; i < BUSH_SIZE; ++i) {
        if(map->boardPage[busheshPlaces[i].y][busheshPlaces[i].x] == FLOOR){
            map->boardPage[busheshPlaces[i].y][busheshPlaces[i].x] = BUSH;
        }
    }
    for (int i = 0; i < CAMPSIDE_SIZE; ++i) {
        if(map->boardPage[campsidePlaces[i].y][campsidePlaces[i].x] == FLOOR){
            map->boardPage[campsidePlaces[i].y][campsidePlaces[i].x] = CAMPSIDE;
        }
    }
}
void drawLegend(int x, int y){
    mvprintw(y, x, "Legend:");
    x++;
    y++;
    for (int i = 0; i < 4; ++i) {
        drawObject(PLAYER+i, y,x+i);
    }
    mvprintw(y++, x+5, "- players");

    drawObject(WALL, y, x);
    mvprintw(y++, x+5, "- wall");
    drawObject(BUSH, y, x);
    mvprintw(y++, x+5, "- bushes (slow down)");
    drawObject(BOT_BEAST, y, x);
    mvprintw(y++, x+5, "- wild beast");
    drawObject(COIN, y, x);
    mvprintw(y++, x+5, "- one coin");
    y++;
    drawObject(TREASURE, y, x);
    mvprintw(y++, x+5, "- tresure (10 coins)");
    drawObject(LARGE_TREASURE, y, x);
    mvprintw(y++, x+5, "- large treasure (50 coins)");
    y++;
    drawObject(CAMPSIDE, y, x);
    mvprintw(y++, x+5, "- campsite");
}