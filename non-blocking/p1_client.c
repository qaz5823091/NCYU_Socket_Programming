#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CHAR 1024
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 5678

int main()
{
    SOCKET client_socket;
    WSADATA data;
    struct sockaddr_in server;
    int i, n;
    char message[MAXIMUM_CHAR] = "I love NP!";

    WSAStartup(0x101, &data);
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    printf("Connect to server...\n");

    u_long mode = 1;
    ioctlsocket(client_socket, FIONBIO, &mode);

    while (1) {
        strcpy(message, "I miss you!");
        printf("(Client): %s\n", message);

        send(client_socket, message, strlen(message) + 1, 0);
        Sleep(500);

        memset(message, 0, MAXIMUM_CHAR);
        n = recv(client_socket, message, MAXIMUM_CHAR, 0);

        if (n > 0) {
            message[n] = '\0';
            printf("(Server): %s\n", message);
        }
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
