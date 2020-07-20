#include "net_hdr.h"

int client_sock_init(char *addr, short SERVER_PORT)
{
	int sockfd;
	struct sockaddr_in serveraddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error!\n");
		exit(-1);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, addr, &serveraddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s\n", addr);
		closesocket(sockfd);
		exit(-2);
	}
	if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0)
	{
		printf("connect error!\n");
		exit(-3);
	}

	return sockfd;
}

void snd_all_msg(int fd)
{
	int i,n;
	char recvline[MAXLINE];
	char *bufs[] = { "I love this!","hi! there!","please tell me","do you know?" };
	for (i = 0; i < sizeof(bufs) / sizeof(char *); i++)
	{
		printf("send msg:%s\n", bufs[i]);
		send(fd, bufs[i], strlen(bufs[i]), 0);
		if ((n = recv(fd, recvline, MAXLINE, 0)) > 0)
		{
			recvline[n] = 0;
			printf("recived:%s\n", recvline);
		}
	}
}

int start_client(int argc, char *argv[])
{
	int sockfd;
	if (argc != 2)
	{
		printf("usage:%s <IPv4 address>\n", argv[0]);
		return -1;
	}

	socklib_init();
	sockfd = client_sock_init(argv[1], 10056);
	snd_all_msg(sockfd);
	sock_cleanup(sockfd);

	return 0;
}
