#include "clientFunc.h"
int moveClick = 0;
int sock;
int lostConnection=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER, waitMutex = PTHREAD_MUTEX_INITIALIZER;

void sendMess(const struct player *players) {
    if(!players)
        return;
    int error = (int) send(sock, &moveClick, sizeof(int), 0);
    if (error < 0) {
        lostConnection=1;
        pthread_mutex_unlock(&waitMutex);
        return;
    }
}
void* rMess(void* arg) {
    struct player *player = (struct player *) arg;
    if (!player)
        return NULL;
    while (1) {
        int error = (int) recv(sock, player, sizeof(struct player), 0);
        if (error <= 0) {
            pthread_mutex_lock(&mutex);
            lostConnection = 1;
            pthread_mutex_unlock(&mutex);
            pthread_mutex_unlock(&waitMutex);
            return NULL;
        }
        pthread_mutex_unlock(&waitMutex);
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
void drawBoard(const struct player* player){
    if(!player)
        return;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int x=i, y=j;
            drawObject(*(*(player->map + i) + j), x, y);
        }
    }
    refresh();
}

void *setClick(void *arg) {
    int *clickMove = (int *) arg;
    while (1) {
        *clickMove = getch();
    }
    return NULL;
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
void drawPlayerInfo(int x, int y, struct player* players) {

    for (int i = 0; i < 16; ++i) {
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

    mvprintw(y+1,x, "                    ");
    mvprintw(y+2,x, "                    ");
    mvprintw(y+3,x, "                    ");
    mvprintw(y+4,x, "                    ");
    mvprintw(y+7,x, "                    ");
    mvprintw(y+8,x, "                    ");

    mvprintw(y,  x, "Player%d", players->ID);
    mvprintw(y+1,x, "%d", players->PID);
    mvprintw(y+2,x, "HUMAN");
    mvprintw(y+3,x, "%d/%d",players->x, players->y);
    mvprintw(y+4,x, "%d",players->deaths);
//
    mvprintw(y+7,x, "%d",players->coins);
    mvprintw(y+8,x, "%d",players->coinsInCamp);

    drawLegend(x+14, y-3);
    refresh();
}