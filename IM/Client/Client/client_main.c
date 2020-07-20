#include "im_client.h"

void socklib_init()
{
	WSADATA wsa_data;
	int ret;

	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0)
	{
		printf("client: windows socket load failed with error code: %d\n", ret);
		exit(-1);
	}
}

void sock_cleanup(int fd)
{
	closesocket(fd);
	WSACleanup();
}

int client_sock_init(char *addr)
{
	int sfd;
	struct sockaddr_in serv;

	socklib_init();

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("client: socket error \n");
		return -1;
	}
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, addr, &serv.sin_addr) <= 0)
	{
		printf("client: inet_pton error for %s\n", addr);
		closesocket(sfd);
		return -2;
	}

	/*链接服务器*/

	/*sockaddr
	sockaddr_in
	*/
	if (connect(sfd, (struct sockaddr *)&serv, sizeof(serv)) < 0)
	{
		printf("client: connet error!\n");
		return -3;
	}

	return sfd;
}


int init_args(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "usage:%s <IPv4 server ip>\n",argv[0]);
		return -1;
	}

	/*init client socket fd*/
	myself.w_sockfd = client_sock_init(argv[1]);
	if (myself.w_sockfd < 0)
	{
		return -1;
	}
	return 0;
}


void *client_cli_thread(void *arg)
{
	char line[MAX_LINE_LEN];
	char user_prompt[MAX_PROMPT_LEN];
	int n = 0;

	while (1)
	{
		if (myself.w_id != -1)
		{
			/*
			C:\Users\Oscar>
			im_client(oscar)#
			*/
			_snprintf(user_prompt, MAX_PROMPT_LEN,"%s(%s)#",USER_PROM,myself.w_name);
		}
		else
		{
			/*
			C:\Users\Oscar>
			im_client(unknown)#
			*/
			_snprintf(user_prompt, MAX_PROMPT_LEN, "%s(unknown)#", USER_PROM);
		}

		printf("%s", user_prompt);
		memset(line, 0, sizeof(line));
		if (fgets(line, MAX_LINE_LEN, stdin) == NULL)
		{
			printf("fgets == null");
			break;
		}
		//gets(line);
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
		//printf("line = \"%s\"\n", line);

		/*执行并判断是否是退出指令*/
		//printf("client: entry cmd_process...\n");
		if (usr_cmd_process(line) == -2)
		{
			printf("client: user logout...\n");
			break;
		}
		
	}

	closesocket(myself.w_sockfd);
	printf("client: exit pthread...\n");
	return NULL;

}


void client_main_loop()
{
	int n=0;
	pthread_t client_stdin_pid;
	pthread_create(&client_stdin_pid, NULL, client_cli_thread, NULL);

	///*
	while ((n = recv(myself.w_sockfd, myself.w_buf, MAX_MSG_SIZE, 0)) > 0)
	{
		//printf("client: recv msg len=%d\n", n);
		dec_server_resp(myself.w_buf, n);
	}


	printf("client: recv len=%d, exiting.....\n",n);
	//*/
	
	/*
	while (1)
	{
		n = recv(myself.w_sockfd, myself.w_buf, MAX_MSG_SIZE, 0);
		if (n <= 0)
		{
			continue;
		}
		dec_server_resp(myself.w_buf, n);
	}
	
	*/
}



/*要求只有一个参数：IPv4*/
int main(int argc, char **argv)
{
	/*用户数据结构初始化*/
	init_user_struct(&myself);
	/*参数初始化*/
	if (init_args(argc, argv) < 0)
	{
		return -1;
	}
	/*客户端主循环处理函数*/
	/*多线程*/
	client_main_loop();
	/*清理函数*/
	sock_cleanup(myself.w_sockfd);
	return 0;
}