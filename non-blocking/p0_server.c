#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CHAR 1024
#define TARGET_PORT 5678

int main()
{
    SOCKET server_socket, client_socket;
    int client_length, n, i;
    char message[MAXIMUM_CHAR];

    struct sockaddr_in server, client;
    WSADATA data;

    WSAStartup(0x101, &data);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = 0;
    server.sin_port = htons(TARGET_PORT);

    bind(server_socket, (LPSOCKADDR) &server, sizeof(server));
    listen(server_socket, 5);

    client_length = sizeof(client);

    while (1) {
        printf("Server waits...\n");
        client_socket = accept(server_socket, (LPSOCKADDR) &client, &client_length);
        u_long i_mode = 0;
        ioctlsocket(client_socket, FIONBIO, &i_mode);

        if (i_mode == 0) {
            printf("This is blocking mode.\n");
        }
        else {
            printf("This is non-blocking mode.\n");
        }

        while (1) {
            printf("recv() checks...\n");
            n = recv(client_socket, message, MAXIMUM_CHAR, 0);
            message[n] = '\0';

            if (n > 0) {
                printf("Recv: %s\n", message);
            }

            Sleep(1000);

            int n_error = WSAGetLastError();
            if (n_error != WSAEWOULDBLOCK && n_error !=0) {
                printf("Disconnected! Error code is : %d\n", n_error);
                closesocket(client_socket);
                break;
            }
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
