#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CHAR 1024
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 1234

int main()
{
    SOCKET client_socket, server_socket;
    WSADATA data;
    struct sockaddr_in client, server;
    int i, n, error, activity;
    char message[MAXIMUM_CHAR] = "I love CPP!";

    WSAStartup(0x101, &data);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    listen(server_socket, 5);

    int client_length = sizeof(client);
    printf("Server waits...\n");
    client_socket = accept(server_socket, (LPSOCKADDR) &client, &client_length);
    printf("Client is connected: socket fd is %d, IP is %s, port is %d\n",
           client_socket, inet_ntoa(client.sin_addr) , ntohs(client.sin_port));

    fd_set table;
    while (1) {
        printf("step1. clear all the socket fd set.\n");
        FD_ZERO(&table);

        printf("step2. add client socket to fd set.\n");
        FD_SET(client_socket, &table);

        printf("step3. call select() and wait.\n");
        activity = select(0, &table, NULL, NULL, NULL);

        printf("step4. wake up from select(): %d\n", activity);
        if (activity == SOCKET_ERROR) {
            printf("select call failed with error code: %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(client_socket, &table)) {
            n = recv(client_socket, message, MAXIMUM_CHAR, 0);
            printf("(From client): %s\n", message);

            if (n == 0) {
                printf("Connection closed.\n");
                break;
            }
        }
    }

    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
