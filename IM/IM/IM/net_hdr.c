#include "net_hdr.h"

void socklib_init()
{
	WSADATA wsa_data;
	int ret;

	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0)
	{
		printf("windows socket load failed with error code : %d\n", ret);
		exit(-1);
	}
}

void sock_cleanup(int fd)
{
	closesocket(fd);
	WSACleanup();
}