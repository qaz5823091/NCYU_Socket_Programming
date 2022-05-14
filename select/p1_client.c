#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CHAR 1024
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 1234

int main()
{
    SOCKET client_socket;
    WSADATA data;
    struct sockaddr_in server;
    int i, n, error;
    char message[MAXIMUM_CHAR] = "I love CPP!";

    WSAStartup(0x101, &data);
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    printf("Connect to server.\n");

    while (1) {
        Sleep(10000);
        send(client_socket, message, strlen(message) + 1, 0);
        printf("Send: %s (in 10 seconds)\n", message);
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
