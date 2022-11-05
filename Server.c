#include <stdio.h>
#include "lib/serverFunc.h"

pthread_t playerThread[4];

int main() {
    ///
    char startServerClick;srand(time(NULL));
    printf("Wanna start server (Y/N)? ");
    scanf("%c%*c", &startServerClick);
    if (startServerClick != 'y' && startServerClick != 'Y') {
        return 1;
    }
    // ------------------ CREATE SERVER ------------------------

    char *ip = "127.0.0.1";
    int port = 5555;
    int SERVER_PID = getpid();



    clear();
    setColors();
    int error = serverCreate(ip, port, 4);
    if(error){
        return 1;
    }
    // -------------------- SET UP PLAYERS ---------------------
    for (int i = 0; i < PLAYERS_SIZE; ++i) {
        players[i].ID = i;
        initializePlayer(&players[i], board);
        pthread_create(&playerThread[i], NULL, connectToServer, &players[i]);
    }
    // -------------------- SET UP BEAST ---------------------

    // ---------------- CLickServer ------------------
    pthread_t clickServerThread;
    pthread_create(&clickServerThread, NULL, setClick, &MOVECLICK);

    // ----------------- GAME ------------------------

    initializeBoardFromFile(&board, "Board_32x24.txt");

    drawBoard(board);
    int whereStartStatsX=34;
    int whereStartStatsY=0;


    /// TODO BEAST
    while (1) {
        tour();
        pthread_mutex_lock(&mutex);
        if (MOVECLICK != 0) {
            error = serverClick(MOVECLICK, board);
            if(error == 1)
                break;
            MOVECLICK = 0;
        }
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex);
        updatePlayer(players, board);
        initMapToPlayers(players, board);
        sendMess(players);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        drawBoard(board);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        drawPlayerInfo(whereStartStatsX, whereStartStatsY, players, PLAYERS_SIZE);
        mvprintw(whereStartStatsY+1, whereStartStatsX+1, "Server PID     %d Round %d", SERVER_PID, ROUND);
        mvprintw(whereStartStatsY+2, whereStartStatsX+1, "   Players     %d", CLIENTS);
        pthread_mutex_unlock(&mutex);
        refresh();
    }
    /// Closing everthing
    for (int i = 0; i < PLAYERS_SIZE; ++i) {
        pthread_cancel(playerThread[i]);
        close(players[i].clientSocket);
    }
    freeBoard(&board);
    pthread_cancel(clickServerThread);
    pthread_mutex_destroy(&mutex);
    clear();
    refresh();
    endwin();
    return 0;
}
