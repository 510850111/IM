#include "net_hdr.h"

int time_server()
{
	int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliadrr;
	char buff[MAXLINE];
	time_t ticks;
	WSADATA wsa_data;
	int ret;


	if ((ret == WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0)
	{
		printf("windows socket load failed with error code: %d", ret);
		return -1;
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(10056);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(listenfd, &servaddr, sizeof(servaddr));
	listen(listenfd, 10);

	for (;;)
	{
		len = sizeof(cliadrr);
		printf("Waiting for client connection......\n");
		connfd = accept(listenfd, &cliadrr, &len);
		printf("connection form %s , port is %d\n", inet_ntop(AF_INET, &cliadrr.sin_addr, buff, sizeof(buff)), ntohs(cliadrr.sin_port));

		ticks = time(NULL);
		_snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		send(connfd, buff, strlen(buff), 0);

		closesocket(connfd);
	}

	closesocket(connfd);
	WSACleanup();

}