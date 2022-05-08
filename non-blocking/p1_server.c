#include <stdio.h>
#include <winsock.h>
#include <time.h>

#define MAXIMUM_CHAR 1024
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 5678
#define TIME_INTERVAL 5

int main()
{
    SOCKET server_socket, client_socket;
    WSADATA data;
    struct sockaddr_in server, client;
    int i, n;
    char message[MAXIMUM_CHAR];
    clock_t start, stop;

    WSAStartup(0x101, &data);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    listen(server_socket, 5);

    int client_length = sizeof(client);
    printf("Server waits...\n");
    client_socket = accept(server_socket, (struct sockaddr *) &client, &client_length);
    printf("Client is connected.\n");

    u_long mode = 1;
    ioctlsocket(client_socket, FIONBIO, &mode);

    stop = 0;
    float time;

    while (1) {
        n = recv(client_socket, message, MAXIMUM_CHAR, 0);
        if (n > 0) {
            message[n] = '\0';
            printf("(Client): %s\n", message);
        }

        start = clock();
        time = (start - stop) / CLOCKS_PER_SEC;

        if (time > TIME_INTERVAL) {
            strcpy(message, "I don't want to know.");
            printf("(Server): %s\n", message);
            send(client_socket, message, strlen(message) + 1, 0);
            stop = clock();
        }
        Sleep(1000);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
