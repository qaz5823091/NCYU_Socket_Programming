#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CHAR 1024
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 1234
#define MAXIMUM_CLIENTS 5

int main()
{
    SOCKET client_socket[MAXIMUM_CLIENTS], server_socket, new_socket;
    WSADATA data;
    struct sockaddr_in client, server;
    int i, n, error, activity, number_of_client = 0;
    char message[MAXIMUM_CHAR] = "I like you!";
    int is_ready[MAXIMUM_CLIENTS];
    fd_set read_table, write_table;

    WSAStartup(0x101, &data);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    listen(server_socket, 5);

    int client_length = sizeof(client);

    for (int i=0;i<MAXIMUM_CLIENTS;i++) {
        client_socket[i] = 0;
        is_ready[i] = 0;
    }

    printf("Server waits...\n");

    while (1) {
        printf("step1. clear all the socket fd set.\n");
        FD_ZERO(&read_table);
        FD_ZERO(&write_table);

        printf("step2-1. add server socket to fd set.\n");
        FD_SET(server_socket, &read_table);
        printf("step2-2. add client socket to fd set.\n");
        for (int i=0;i<MAXIMUM_CLIENTS;i++) {
            if (client_socket[i] > 0) {
                if (is_ready[i] == 0) {
                    FD_SET(client_socket[i], &read_table);
                }
                else {
                    FD_SET(client_socket[i], &write_table);
                }
            }
        }

        printf("step3. call select() and wait.\n");
        activity = select(0, &read_table, &write_table, NULL, NULL);

        printf("step4. wake up from select(): %d\n", activity);
        if (activity == SOCKET_ERROR) {
            printf("select call failed with error code: %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_socket, &read_table)) {
            client_length = sizeof(client);
            new_socket = accept(server_socket, (struct sockaddr *) &client, &client_length);
            printf("New connection: socket fd is %d, IP is %s, port is %d\n",
                   new_socket, inet_ntoa(client.sin_addr) , ntohs(client.sin_port));

            for (int i=0;i<MAXIMUM_CLIENTS;i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    printf("The client%d socket is in client_socket[%d]\n", ++number_of_client, i);
                    break;
                }
            }
        }

        for (int i=0;i<MAXIMUM_CLIENTS;i++) {
            if (FD_ISSET(client_socket[i], &read_table)) {
                n = recv(client_socket[i], message, MAXIMUM_CHAR, 0);
                if (n == SOCKET_ERROR) {
                    error = WSAGetLastError();
                    if (error == WSAECONNRESET) {
                        printf("Host disconnected unexpectedly.\n");
                        closesocket(client_socket[i]);
                        client_socket[i] = 0;
                        --number_of_client;
                    }
                    else {
                        printf("Receive failed with error code: %d\n", error);
                    }
                }
                if (n == 0) {
                    printf("Host disconnected.\n");
                    closesocket(client_socket[i]);
                    client_socket[i] = 0;
                    --number_of_client;
                }
                else if (n > 0) {
                    printf("Receive from and echo to client_socket[%d]: %s\n", i, message);
                    is_ready[i] = 1;
                }
            }

            if (FD_ISSET(client_socket[i], &write_table)) {
                send(client_socket[i], message, strlen(message) + 1, 0);
                is_ready[i] = 0;
            }
        }
    }

    closesocket(server_socket);
    for (int i=0;i<MAXIMUM_CLIENTS;i++) {
        closesocket(client_socket[i]);
    }

    WSACleanup();

    return 0;
}
