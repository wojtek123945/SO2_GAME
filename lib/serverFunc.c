#include "serverFunc.h"

enum key_t KeyL[4];
enum clientInBush playerInBush[4];
struct player_t players[PLAYERS_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t beastThread[4];

int ROUND=0;
int MOVECLICK;
int serverSocket;
struct sockaddr_in serverAddr;
int CLIENTS=0;

int serverCreate(const char *ip, int port, int numberOfClient) {
    int n;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        // Can't create socket
        return 1;
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = port;
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    n = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (n < 0) {
        // BIND ERROR
        return 2;
    }

    listen(serverSocket, numberOfClient);
    mvprintw(0, 0, "[*]Server Started");
    refresh();
    return 0;
}

void *connectToServer(void *arg) {
    struct player_t *player = (struct player_t *) (arg);
    if (!player)
        return NULL;
    socklen_t addrSize = sizeof(socklen_t);
    struct sockaddr_in clientAddr;
    player->clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize);
    CLIENTS++;
    player->isConnected = 1;
    player->PID = getpid();
    board->boardPage[player->y][player->x] = PLAYER + player->ID;
    // ---------- RECV MESS ----------
    pthread_t recvPlayerThread;
    pthread_create(&recvPlayerThread, NULL, recvMess, player);
    return NULL;
}

void sendMess(struct player_t player[]) {
    for (int i = 0; i < 4; ++i) {
        if (player[i].isConnected == 1) {
            int error = (int) send(player[i].clientSocket, &player[i], sizeof(struct player_t), 0);
            if (error <= 0) {
                mvprintw(0, 0, "[-Send]Disconnect client. \n");
                refresh();
                closePlayer(&player[i], board);
                player[i].isConnected = 0;
                CLIENTS--;
            }
        }
    }
}

void *recvMess(void *arg) {
    struct player_t *player = (struct player_t *) arg;
    if (!player)
        return NULL;
    while (1) {
        int getMove;
        int error = (int) recv(player->clientSocket, &getMove, sizeof(int), 0);
        if (error <= 0) {
            mvprintw(0, 0, "[-Recv]Disconnect client. \n");
            closePlayer(player, board);
            player->isConnected = 0;
            CLIENTS--;
            return NULL;
        }
        setKeyL(getMove, player->ID);
    }
}

void setKeyL(int playerMove, int playerId) {
    pthread_mutex_lock(&mutex);
    switch (playerMove) {
        case 'W':
        case 'w':
            KeyL[playerId] = UP;
            break;
        case 'S':
        case 's':
            KeyL[playerId] = DOWN;
            break;
        case 'A':
        case 'a':
            KeyL[playerId] = LEFT;
            break;
        case 'D':
        case 'd':
            KeyL[playerId] = RIGHT;
            break;
        default:
            break;
    }
    pthread_mutex_unlock(&mutex);
}

void *setClick(void *arg) {
    int *clickMove = (int *) arg;
    while (1) {
        *clickMove = getch();
    }
}

void tour() {
    usleep(1000 * 100 * MILI_SECOND_PER_ROUND);
    ROUND++;
}

int serverClick(int moveClick, struct board_t *map) {
    switch (moveClick) {
        case 0: {
            return 0;
        }
        case 'c':
        case 'C': {
            int randomNumber = (int) (rand() % 100) + 1;
            if (randomNumber < 50)
                generateCoin(map, COIN);
            else if (randomNumber < 80 && randomNumber >= 50)
                generateCoin(map, TREASURE);
            else
                generateCoin(map, LARGE_TREASURE);
            return 0;
        }
        case 'q':
        case 'Q': {
            return 1;
        }
        case 'B':
        case 'b':{
            if(actualBeastSize < 4){
                pthread_create(&beastThread[actualBeastSize], NULL, startBeast, &beast[actualBeastSize]);
            }
        }
        default:
            break;
    }
    return 0;
}

void generateCoin(struct board_t *map, enum boardObstacles obstacles) {
    if (!map)
        return;
    // 6 vs 1
    int x, y;
    if(boardFreeSpace <=0)
        return;
    do {
        y = (int) (rand() % map->width - 1) + 1;
        x = (int) (rand() % map->height - 1) + 1;
        if (*(*(map->boardPage + x) + y) == ' ')
            break;
    } while (1);
    if (obstacles == COIN) {
        drawObject(COIN, x, y);
        *(*(map->boardPage + x) + y) = 'c';
    } else if (obstacles == TREASURE) {
        drawObject(TREASURE, x, y);
        *(*(map->boardPage + x) + y) = 't';
    } else {
        drawObject(LARGE_TREASURE, x, y);
        *(*(map->boardPage + x) + y) = 'T';
    }
    refresh();
}