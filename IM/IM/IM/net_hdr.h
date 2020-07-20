#ifndef NET_HDR_H
#define NET_HDR_H

#include<WinSock2.h>
#include<stdio.h>
#include<windows.h>
#include<WS2tcpip.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#pragma comment(lib,"Ws2_32.lib")
#define MAXLINE 80

void socklib_init();
void sock_cleanup(int fd);

#endif // !NET_HDR_H
