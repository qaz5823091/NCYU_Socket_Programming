#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CHAR 1024
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 5678

int main()
{
    SOCKET client_socket;
    struct sockaddr_in server;
    char message[MAXIMUM_CHAR] = "How are you?";
    WSADATA data;
    int n, i;

    WSAStartup(0x101, (LPWSADATA) &data);
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_port = htons(TARGET_PORT);

    connect(client_socket, (LPSOCKADDR) &server, sizeof(server));
    printf("Client has connected to server.\n");
    printf("Waiting 10 seconds on purpose...\n");
    Sleep(10000);

    while (1) {
        Sleep(3000);
        send(client_socket, message, strlen(message) + 1, 0);

        int error_code = WSAGetLastError();
        if (error_code != WSAEWOULDBLOCK && error_code != 0) {
            printf("Winsock error code is %d\n", error_code);
            printf("Disconnected!\n");
            closesocket(client_socket);
            break;
        }
        printf("Send every 3 seconds: %s\n", message);
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
