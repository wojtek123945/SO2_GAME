#include "playerFunc.h"

int validatePlayerMove(int x, int y, struct board_t *map) {
    if (!map)
        return -1;
    if (*(*(map->boardPage + y) + x) != WALL)
        return 0;
    return 1;
}

void drawPlayerInfo(int x, int y, struct player_t player[], int maxSizePlayer) {

    for (int i = 0; i < 26; ++i) {
        mvprintw(y+i, x, "|");
    }
    x++;
    mvprintw(y, x, "---STATS---------------------------------------");
    y+=4;
    mvprintw(y,x,   "Parameter:");
    mvprintw(y+1,x, " PID");
    mvprintw(y+2,x, " TYPE");
    mvprintw(y+3,x, " Curr X/Y");
    mvprintw(y+4,x, " Deaths");
    mvprintw(y+6,x, " Coins:");
    mvprintw(y+7,x, "   carried");
    mvprintw(y+8,x, "   brought");
    x+=13;
    for (int i = 0; i < maxSizePlayer; ++i) {
        if(player[i].isConnected){
            mvprintw(y+1,x+i*9, "      ");
            mvprintw(y+2,x+i*9, "      ");
            mvprintw(y+3,x+i*9, "      ");
            mvprintw(y+4,x+i*9, "      ");
            mvprintw(y+7,x+i*9, "      ");
            mvprintw(y+8,x+i*9, "      ");

            mvprintw(y,x+i*9, "Player%d", player[i].ID);
            mvprintw(y+1,x+i*9, "%d", player[i].PID);
            mvprintw(y+2,x+i*9, "HUMAN");
            mvprintw(y+3,x+i*9, "%2d/%2d", player[i].x, player[i].y);
            mvprintw(y+4,x+i*9, "%d", player[i].deaths);

            mvprintw(y+7,x+i*9, "%d", player[i].coins);
            mvprintw(y+8,x+i*9, "%d", player[i].coinsInCamp);
            continue;
        }
        if(player[i].isConnected == 0){
            mvprintw(y+1,x+i*9, "      ");
            mvprintw(y+2,x+i*9, "      ");
            mvprintw(y+3,x+i*9, "      ");
            mvprintw(y+4,x+i*9, "      ");
            mvprintw(y+7,x+i*9, "      ");
            mvprintw(y+8,x+i*9, "      ");

            mvprintw(y,x+i*9, "Player%d", player[i].ID);
            mvprintw(y+1,x+i*9, "-");
            mvprintw(y+2,x+i*9, "-");
            mvprintw(y+3,x+i*9, "--/--");
            mvprintw(y+4,x+i*9, "-");
        }
    }
    drawLegend(x, y+11);
    refresh();
}

void initializePlayer(struct player_t *player, struct board_t *map) {
    if (!player)
        return;
    //x =29, y =11
    player->x = 29;
    player->y = 10 + player->ID;
    player->isConnected = 0;
}

void closePlayer(struct player_t *player, struct board_t* map){
    if (!player)
        return;
    player->isConnected = 0;
    //close(player->clientSocket);
    map->boardPage[player->y][player->x] = FLOOR;
    player->y = 0;
    player->x = 0;
    player->deaths = 0;
    player->coinsInCamp = 0;
    player->coins = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            player->map[i][j] = 0;
        }
    }
    close(player->clientSocket);
}

void initMapToPlayers(struct player_t player[], const struct board_t *map) {
    if (!player)
        return;
    for (int i = 0; i < PLAYERS_SIZE; ++i) {
        if (player[i].isConnected == 1) {
            int playerX = player[i].x;
            int playerY = player[i].y;
            for (int y = 0; y < 5; ++y) {
                for (int x = 0; x < 5; ++x) {
                    int mapX = (playerX-2) + x;
                    int mapY = (playerY-2) + y;
                    player[i].map[y][x] = (char) map->boardPage[mapY][mapX];
                }
            }
        }
    }
}

void updatePlayer(struct player_t player[], const struct board_t *map) {
    if (!player)
        return;
    for (int i = 0; i < PLAYERS_SIZE; ++i) {
        if (player[i].isConnected == 1) {
            if (KeyL[player[i].ID] == UP) {
                if (validatePlayerMove(player[i].x, player[i].y - 1, board) == 0){
                    if(ifPlayerInBush(&player[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(playerInBush[player[i].ID] == IN_BUSH){
                            playerInBush[player[i].ID] = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[player[i].y][player[i].x] = FLOOR;
                    player[i].y--;
                    int obj = board->boardPage[player[i].y][player[i].x];
                    if(obj == BUSH){
                        playerInBush[player[i].ID] = IN_BUSH;
                    }
                    updatePlayerCoins(&player[i], obj);
                    board->boardPage[player[i].y][player[i].x] = PLAYER + player[i].ID;
                }
            } else if (KeyL[player[i].ID] == DOWN) {
                if (validatePlayerMove(player[i].x, player[i].y + 1, board) == 0){
                    if(ifPlayerInBush(&player[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(playerInBush[player[i].ID] == IN_BUSH){
                            playerInBush[player[i].ID] = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[player[i].y][player[i].x] = FLOOR;
                    player[i].y++;
                    int obj = board->boardPage[player[i].y][player[i].x];
                    if(obj == BUSH){
                        playerInBush[player[i].ID] = IN_BUSH;
                    }
                    updatePlayerCoins(&player[i], obj);
                    board->boardPage[player[i].y][player[i].x] = PLAYER + player[i].ID;
                }
            } else if (KeyL[player[i].ID] == LEFT) {
                if (validatePlayerMove(player[i].x - 1, player[i].y, board) == 0){
                    if(ifPlayerInBush(&player[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(playerInBush[player[i].ID] == IN_BUSH){
                            playerInBush[player[i].ID] = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[player[i].y][player[i].x] = FLOOR;
                    player[i].x--;
                    int obj = board->boardPage[player[i].y][player[i].x];
                    if(obj == BUSH){
                        playerInBush[player[i].ID] = IN_BUSH;
                    }
                    updatePlayerCoins(&player[i], obj);
                    board->boardPage[player[i].y][player[i].x] = PLAYER + player[i].ID;
                }
            } else if (KeyL[player[i].ID] == RIGHT) {
                if (validatePlayerMove(player[i].x + 1, player[i].y, board) == 0){
                    if(ifPlayerInBush(&player[i], busheshPlaces) == 1){
                        //Player in bush check if he was
                        if(playerInBush[player[i].ID] == IN_BUSH){
                            playerInBush[player[i].ID] = OUT_BUSH;
                            continue;
                        }
                    }
                    board->boardPage[player[i].y][player[i].x] = FLOOR;
                    player[i].x++;
                    int obj = board->boardPage[player[i].y][player[i].x];
                    if(obj == BUSH){
                        playerInBush[player[i].ID] = IN_BUSH;
                    }
                    updatePlayerCoins(&player[i], obj);
                    board->boardPage[player[i].y][player[i].x] = PLAYER + player[i].ID;
                }
            }
            KeyL[player[i].ID] = NONE;
        }
    }
    if(CLIENTS > 1) {
        for (int i = 0; i < PLAYERS_SIZE; ++i) {
            if (player[i].isConnected == 1) {
                for (int j = i + 1; j < PLAYERS_SIZE; ++j) {
                    if (player[j].isConnected == 1) {
                        if (player[i].y == player[j].y && player[i].x == player[j].x) {
                            playerColison(&player[i], &player[j], board);
                        }
                    }
                }
            }
        }
    }

}

void playerColison(struct player_t* player_1, struct player_t* player_2, struct board_t* map){
    if(!player_1 || !player_2)
        return;
    if(coinBag_15.size<COIN_BAG_SIZE) {
        int lastSize = coinBag_15.size;
        coinBag_15.bag[lastSize].coord.x = player_1->x;
        coinBag_15.bag[lastSize].coord.y = player_1->y;
        coinBag_15.bag[lastSize].coins += player_1->coins;
        coinBag_15.bag[lastSize].coins += player_2->coins;
        player_1->coins = 0;
        player_2->coins = 0;
        coinBag_15.size++;
    }else{
        player_1->x = 29;
        player_1->y = 10+player_1->ID;
        board->boardPage[player_1->y][player_1->x] = PLAYER+player_1->ID;

        player_2->x = 29;
        player_2->y = 10+player_2->ID;
        board->boardPage[player_2->y][player_2->x] = PLAYER+player_2->ID;

        player_1->deaths++;
        player_2->deaths++;
        return;
    }

    board->boardPage[player_1->y][player_1->x] = COIN_BAG;

    player_1->x = 29;
    player_1->y = 10+player_1->ID;
    board->boardPage[player_1->y][player_1->x] = PLAYER+player_1->ID;

    player_2->x = 29;
    player_2->y = 10+player_2->ID;
    board->boardPage[player_2->y][player_2->x] = PLAYER+player_2->ID;

    player_1->deaths++;
    player_2->deaths++;

}

void updatePlayerCoins(struct player_t* player, int obj){
    if(!player)
        return;
    if(obj == COIN){
        player->coins++;
    }else if(obj == TREASURE){
        player->coins+=10;
    }else if(obj == LARGE_TREASURE){
        player->coins+=50;
    }else if(obj == CAMPSIDE){
        player->coinsInCamp+=player->coins;
        player->coins=0;
    }else if(obj == COIN_BAG){
        for (int i = 0; i < coinBag_15.size; ++i) {
            if(coinBag_15.bag[i].coord.x == player->x && coinBag_15.bag[i].coord.y == player->y){
                player->coins+=coinBag_15.bag[i].coins;

                int lastCoinBagID = coinBag_15.size-1;
                int newX = coinBag_15.bag[lastCoinBagID].coord.x;
                int newY = coinBag_15.bag[lastCoinBagID].coord.y;

                coinBag_15.bag[i].coins = coinBag_15.bag[lastCoinBagID].coins;
                coinBag_15.bag[i].coord.x = newX;
                coinBag_15.bag[i].coord.y = newY;

                coinBag_15.bag[lastCoinBagID].coord.x = 0;
                coinBag_15.bag[lastCoinBagID].coord.y = 0;
                coinBag_15.bag[lastCoinBagID].coins = 0;
                coinBag_15.size--;
                break;
            }
        }
    }
}

int ifPlayerInBush(const struct player_t *player,const struct coordination_t* bushCoord){
    if(!player || !bushCoord)
        return -1;
    for (int i = 0; i < BUSH_SIZE; ++i) {
        if(player->x == bushCoord[i].x && player->y == bushCoord[i].y)
            return 1;
    }
    return 0;
}