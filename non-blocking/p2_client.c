#include <stdio.h>
#include <winsock.h>
#include <time.h>

int main(){
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv;
	int i,n;
	char str[100]="I love NP!";
	char str1[100];

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sd, (struct sockaddr *)  &serv,sizeof(serv) );
	printf("Connect to server.\n");

	u_long mode = 1;
	ioctlsocket(sd, FIONBIO, &mode);

	printf("Client name:");
	gets(str1);
	printf("Time interval(second):");
	int interval;
	scanf("%d", &interval);
	Sleep(5000);

	clock_t start, stop;
	stop = 0;
	float time;

	while(1){
        start = clock();
        time = (start - stop) / CLOCKS_PER_SEC;
        if (time > interval) {
            strcpy(str, str1);
            printf("[Client 1]: %s\n", str);
            send(sd, str, strlen(str) + 1, 0);
            stop = clock();
        }
		memset(str,0,100);
		n = recv(sd, str, 100, 0);
		if (n > 0) {
            str[n] = '\0';
            printf("[Others]: %s\n", str);
		}

        Sleep(100);
	}

	closesocket(sd);
	WSACleanup();

    return 0;
}
