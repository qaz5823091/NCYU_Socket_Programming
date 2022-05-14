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

    fd_set readfds;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        n = select(0, &readfds, NULL, NULL, NULL);
        if (FD_ISSET(client_socket, &readfds)) {
            n = recv(client_socket, message, MAXIMUM_CHAR, 0);
        }
        if (n > 0) {
            printf("(From server): %s\n", message);
        }
        else {
            error = WSAGetLastError();
            if (n != 0 && error != WSAEWOULDBLOCK) {
                break;
            }
        }
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
