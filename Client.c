#include "lib/clientFunc.h"

int main() {

    // ---------------- SETTINGS -------------
    char *ip = "127.0.0.1";
    int port = 6666;

    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 1;
    }

    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    int error;
    while (1) {
        error = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
        if (error >= 0)
            break;
        char d;
        printf("[*]Not connected Wanna try again (Y/N)?\n");
        scanf("%c%*c", &d);
        if (d == 'Y' || d == 'y')
            continue;
        else {
            printf("Bye :D");
            exit(1);
        }
    }
    printf("Connected to the Server.\n");

    pthread_t clickClientThread;
    pthread_create(&clickClientThread, NULL, setClick, &moveClick);
    clear();
    setColors();

    // ---------------------- GAME -----------------------
    struct player player;
    pthread_t playerRecvThread;
    pthread_create(&playerRecvThread, NULL, rMess, &player);

    while (1) {
        pthread_mutex_lock(&waitMutex);

        pthread_mutex_lock(&mutex);
        if(lostConnection == 1){
            break;
        }
        pthread_mutex_unlock(&mutex);

        drawBoard(&player);
        pthread_mutex_lock(&mutex);
        if (moveClick != 0) {
            if(moveClick == 'q' || moveClick == 'Q'){
                pthread_mutex_unlock(&mutex);
                break;
            }
            sendMess(&player);
            moveClick = 0;
        }else
            sendMess(&player);
        pthread_mutex_unlock(&mutex);
        drawPlayerInfo(10, 0, &player);
        refresh();
    }
    clear();
    refresh();
    pthread_cancel(clickClientThread);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&waitMutex);
    close(sock);
    printf("[-]Disconnected from the server.\n");
    return 0;
}

