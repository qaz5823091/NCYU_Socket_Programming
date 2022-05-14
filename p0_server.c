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
    int i, n, error;
    char message[MAXIMUM_CHAR] = "I love CPP!";

    WSAStartup(0x101, &data);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    listen(server_socket, 5);

    int client_length = sizeof(client);
    while (1) {
        printf("Server waits...\n");
        client_socket = accept(server_socket, (struct sockaddr *) &client, &client_length);
        printf("Client is connected.\n");
        while (1) {
            n = send(client_socket, message, strlen(message) + 1, 0);
            if (n < 0) {
                closesocket(client_socket);
                break;
            }
            printf("(Server send): %s\n", message);
            Sleep(1000);
        }
    }

    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
