#include "beastFunc.h"

struct beast_t beast[BEAST_SIZE];
int actualBeastSize = 0;

//Init Beast Server Func
void initBeast(){
    for (int i = 0; i < BEAST_SIZE; ++i) {
        beast[i].isActive=0;
        beast[i].ID = i;
        // TODO sprawdzić mutex przy tworzeniu
        pthread_mutex_init(&beast[i].beastMutex, NULL);
    }
}
void generateBeastLocation(struct board_t *map, struct beast_t* beastClient) {
    if (!map)
        return;
    int x, y;
    if(boardFreeSpace <= 0)
        return;
    do {
        y = (int) (rand() % (map->height - 1) + 1);
        x = (int) (rand() % (map->width - 1) + 1);
        if (*(*(map->boardPage + y) + x) == FLOOR)
            break;
    } while (1);
    beastClient->x = x;
    beastClient->y = y;
    refresh();
}

//Thread Func
void* startBeast(void* arg){
    struct beast_t* beastClient = (struct beast_t*)arg;
    if(!beastClient)
        return NULL;
    actualBeastSize++;
    beast[beastClient->ID].PID = getpid();
    beast[beastClient->ID].isActive = 1;
    generateBeastLocation(board, &beast[beastClient->ID]);
    board->boardPage[beastClient->y][beastClient->x] = BOT_BEAST;

    while (1){
        checkForPlayer(beastClient);
        pthread_mutex_lock(&beastClient->beastMutex);
    }
}
int checkForPlayer(struct beast_t* beastClient){
    if(!beastClient)
        return 0;
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            if(beastClient->map[y][x] >= PLAYER && beastClient->map[y][x] <= PLAYER_4){

                beastMove(beastClient, x, y);
                return 1;
            }
        }
    }
    return 0;
}
void beastMove(struct beast_t* beastClient, int playerX, int playerY){
    int beastActualX = beastClient->x;
    int beastActualY = beastClient->y;
    mvprintw(34, 1, "X %d Y %d", playerX, playerY);
    if(playerX == 2){
        //góra dół ruch
        if(playerY > 2){
            if(beastClient->map[beastActualY+1][beastActualX] != WALL){
                beastClient->beastMove = DOWN;
                mvprintw(32, 1, "Beastmove DOWN");
                return;
            }
        }else{
            if(beastClient->map[beastActualY-1][beastActualX] != WALL){
                beastClient->beastMove = UP;
                mvprintw(32, 1, "Beastmove UP");
                return;
            }
        }
    }
    if(playerY == 2){
        if(playerX > 2){
            if(beastClient->map[beastActualY][beastActualX+1] != WALL){
                beastClient->beastMove = RIGHT;
                mvprintw(32, 1, "Beastmove RIGHT");
                return;
            }
        }else{
            if(beastClient->map[beastActualY][beastActualX-1] != WALL){
                mvprintw(32, 1, "Beastmove LEFT");
                beastClient->beastMove = LEFT;
                return;
            }
        }
    }

    //po Lewej stronie besti mapy
    if(playerX > 2){
        //Lewa góra część mapy
        if(playerY > 2){
            if(beastClient->map[beastActualY+1][beastActualX] != WALL){
                mvprintw(32, 1, "Beastmove DOWN");
                beastClient->beastMove = DOWN;
                return;
            }else if(beastClient->map[beastActualY][beastActualX+1] != WALL){
                mvprintw(32, 1, "Beastmove RIGTH");
                beastClient->beastMove = RIGHT;
                return;
            }else{
                generateRandMove(beastClient);
                return;
            }
        }else{
            //Lewa dolna czesc mapy
            if(beastClient->map[beastActualY-1][beastActualX] != WALL){
                mvprintw(32, 1, "Beastmove UP");
                beastClient->beastMove = UP;
                return;
            }else if(beastClient->map[beastActualY][beastActualX+1] != WALL){
                mvprintw(32, 1, "Beastmove RIGHT");
                beastClient->beastMove = RIGHT;
                return;
            }else{
                generateRandMove(beastClient);
                return;
            }
        }
    }else{
        if(playerY > 2){
            if(beastClient->map[beastActualY+1][beastActualX] != WALL){
                mvprintw(32, 1, "Beastmove DOWN");
                beastClient->beastMove = DOWN;
                return;
            }else if(beastClient->map[beastActualY][beastActualX-1] != WALL){
                mvprintw(32, 1, "Beastmove LEFT");
                beastClient->beastMove = LEFT;
                return;
            }
            else{
                generateRandMove(beastClient);
                return;
            }
        }else{
            //Lewa dolna czesc mapy
            if(beastClient->map[beastActualY-1][beastActualX] != WALL){
                mvprintw(32, 1, "Beastmove UP");
                beastClient->beastMove = UP;
                return;
            }else if(beastClient->map[beastActualY][beastActualX-1] != WALL){
                mvprintw(32, 1, "Beastmove LEFT");
                beastClient->beastMove = LEFT;
                return;
            } else{
                generateRandMove(beastClient);
            }
        }
    }
}
void generateRandMove(struct beast_t* beastClient) {
    int beastActualX = beastClient->x;
    int beastActualY = beastClient->y;
    while (1) {
        enum key_t move = rand() % 5 + 1;
        switch (move) {
            case UP:
                if (beastClient->map[beastActualY - 1][beastActualX] != WALL) {
                    beastClient->beastMove = UP;
                    return;
                }
            case DOWN:
                if (beastClient->map[beastActualY + 1][beastActualX] != WALL) {
                    beastClient->beastMove = DOWN;
                    return;
                }
            case LEFT:
                if (beastClient->map[beastActualY][beastActualX - 1] != WALL) {
                    beastClient->beastMove = LEFT;
                    return;
                }
            case RIGHT:
                if (beastClient->map[beastActualY][beastActualX + 1] != WALL) {
                    beastClient->beastMove = RIGHT;
                    return;
                }
        }
    }
}
//Server Beast Func
void updateBeast(struct beast_t beasts[]){
    for (int i = 0; i < BEAST_SIZE; ++i) {
        if(beasts[i].isActive){
            // TODO walidacja ruchów besti
            if(beasts[i].beastMove == UP){
                if (validateBeastMove(beasts[i].x, beasts[i].y - 1, board) == 0){
                    if(ifBeastInBush(&beasts[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(beasts[i].beastInBush == IN_BUSH){
                            beasts[i].beastInBush = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = FLOOR;
                    beasts[i].y--;
                    int obj = board->boardPage[beasts[i].y][beasts[i].x];
                    if(obj == BUSH){
                        beasts[i].beastInBush = IN_BUSH;
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = BOT_BEAST;
                }
            }else if(beasts[i].beastMove == DOWN){
                if (validateBeastMove(beasts[i].x, beasts[i].y + 1, board) == 0){
                    if(ifBeastInBush(&beasts[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(beasts[i].beastInBush == IN_BUSH){
                            beasts[i].beastInBush = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = FLOOR;
                    beasts[i].y++;
                    int obj = board->boardPage[beasts[i].y][beasts[i].x];
                    if(obj == BUSH){
                        beasts[i].beastInBush = IN_BUSH;
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = BOT_BEAST;
                }
            }else if(beasts[i].beastMove == LEFT){
                if (validateBeastMove(beasts[i].x - 1, beasts[i].y, board) == 0){
                    if(ifBeastInBush(&beasts[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(beasts[i].beastInBush == IN_BUSH){
                            beasts[i].beastInBush = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = FLOOR;
                    beasts[i].x--;
                    int obj = board->boardPage[beasts[i].y][beasts[i].x];
                    if(obj == BUSH){
                        beasts[i].beastInBush = IN_BUSH;
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = BOT_BEAST;
                }
            }else if(beasts[i].beastMove == RIGHT){
                if (validateBeastMove(beasts[i].x + 1, beasts[i].y, board) == 0){
                    if(ifBeastInBush(&beasts[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(beasts[i].beastInBush == IN_BUSH){
                            beasts[i].beastInBush = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = FLOOR;
                    beasts[i].x++;
                    int obj = board->boardPage[beasts[i].y][beasts[i].x];
                    if(obj == BUSH){
                        beasts[i].beastInBush = IN_BUSH;
                    }
                    board->boardPage[beasts[i].y][beasts[i].x] = BOT_BEAST;
                }
            }
            beasts[i].beastMove = NONE;
        }
    }
}
void updateBeastMap(struct beast_t* beastClient,struct board_t* map){
    if(!map)
        return;
    int beastX = beastClient->x;
    int beastY = beastClient->y;
    if(beastX < 2 || beastY < 2)
        return;
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            int mapX = (beastX-2) + x;
            int mapY = (beastY-2) + y;
            beastClient->map[y][x] = (char) board->boardPage[mapY][mapX];
        }
    }
}
int validateBeastMove(int x, int y, struct board_t *map) {
    if (!map)
        return -1;
    if (*(*(map->boardPage + y) + x) != WALL)
        return 0;
    return 1;
}
int ifBeastInBush(const struct beast_t *beastClient,const struct coordination_t* bushCoord){
    if(!beastClient || !bushCoord)
        return -1;
    for (int i = 0; i < BUSH_SIZE; ++i) {
        if(beastClient->x == bushCoord[i].x && beastClient->y == bushCoord[i].y)
            return 1;
    }
    return 0;
}