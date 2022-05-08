#include <stdio.h>
#include <winsock.h>

int main(){
	SOCKET sd;
	WSADATA wsadata;
	struct sockaddr_in serv;
	int i,n;
	char str[100]="I love NP!";

	WSAStartup(0x101,&wsadata);
	sd = socket(AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sd, (struct sockaddr *)  &serv,sizeof(serv) );
	printf("Connect to server.\n");

	while(1){

		printf("[Client 1]:");
		gets(str);
		send(sd,str, strlen(str)+1,0);
		memset(str,0,100);
		recv(sd,str, 100, 0);
		printf("[Client 2]:%s\n",str);

	}
	closesocket(sd);
	WSACleanup();

}
