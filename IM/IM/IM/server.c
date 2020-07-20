#include "net_hdr.h"

/*·şÎñÆ÷¶Ë*/
/*¼àÌısocket

server_socket_init();  SERVER_PORT exit(-1);
server_main_loop();  recv  send 
mian();

*/

int server_sock_init(short SERVER_PORT)
{
	struct sockaddr_in serveraddr;
	int ret, fd;

	if ((fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("socket creat error!\n");
		exit(-1);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	if ((ret = bind(fd, &serveraddr, sizeof(serveraddr)) < 0))
	{
		printf("bind return error!\n");
		exit(-2);
	}
	if ((ret = listen(fd, 10)) < 0)
	{
		printf("listen return error!\n");
		exit(-3);
	}

	return fd;
}

void server_main_loop(int listenfd)
{
	int connfd, n;
	socklen_t len;
	struct sockaddr_in cliaddr;
	char buff[MAXLINE];

	while (1)
	{
		len = sizeof(cliaddr);
		printf("waitting for client's connection......\n");
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
		printf("connect from %s , port is %d \n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
		
		while ((n = recv(connfd, buff, MAXLINE, 0)) > 0)
		{
			send(connfd, buff, n, 0);
		}
		closesocket(connfd);
	}
}

int serve_startup()
{

	int listenfd;

	socklib_init();
	listenfd = server_sock_init(10056);
	server_main_loop(listenfd);

	sock_cleanup(listenfd);

	return 0;
}
