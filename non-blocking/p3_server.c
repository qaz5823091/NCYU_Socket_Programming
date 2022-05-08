#include <stdio.h>
#include <winsock.h>

#define MAXIMUM_CLIENTS 3

int main(){
	SOCKET sd,clnt_sd[MAXIMUM_CLIENTS];
	WSADATA wsadata;
	struct sockaddr_in serv,clnt;
	int i,n;
	char str[100]="I love CPP!";

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sd, (struct sockaddr *) &serv, sizeof(serv));

	listen(sd,5);

	int clnt_len=sizeof(clnt);
	printf("Server waits.\n");

	u_long mode = 1;
	ioctlsocket(sd, FIONBIO, &mode);

	for (int i=0;i<MAXIMUM_CLIENTS;i++) {
        clnt_sd[i] = -1;
	}

	int num = 0;

	while(1){
        n = accept(sd, (struct sockaddr *) &clnt, &clnt_len);
        if (n != -1) {
            clnt_sd[num] = n;
            printf("Client[%d] is connected.\n", num);
            num++;
        }
        for (int i=0;i<MAXIMUM_CLIENTS;i++) {
            if (clnt_sd[i] != -1) {
                send(clnt_sd[i], str, strlen(str) + 1, 0);
                printf("Send to clnt[%d]: %s\n", i, str);
            }
        }
		Sleep(1000);
	}
	for (int i=0;i<MAXIMUM_CLIENTS;i++) {
        if (clnt_sd[i] != -1) {
            closesocket(clnt_sd[i]);
        }
    }
	closesocket(sd);
	WSACleanup();
	system("pause");

}
