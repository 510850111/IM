#include "net_hdr.h"

int client_startup(int argc, char *argv[])
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	WSADATA wsa_data;
	int ret;


	if (argc != 2)
	{
		printf("usage %s <IPv4 address>\n", argv[1]);
		return -1;
	}

	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0)
	{
		printf("windows socket load failed with error code: %d\n", ret);
		return -1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error with code: %d\n", sockfd);
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(10056);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s \n", argv[1]);
		closesocket(sockfd);
		return -1;
	}

	if (connect(sockfd, &servaddr, sizeof(servaddr)) < 0)
	{
		printf("connect error!\n");
		closesocket(sockfd);
		return -1;
	}

	while ((n = recv(sockfd, recvline, MAXLINE, 0)) > 0)
	{
		recvline[n] = 0;
		fputs(recvline, stdout);
	}

	closesocket(sockfd);
	WSACleanup();

	return 0;
}