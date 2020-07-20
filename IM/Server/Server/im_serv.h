#pragma once
#ifndef IM_SERV_H
#define IM_SERV_H

#include <WinSock2.h>
#include <stdio.h>
#include <windows.h>
#include <Ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "pthreadVC1.lib")

#include "list.h"
#include "msg_type.h"
#include "serv_config.h"
#include "serv_dec.h"
#include "serv_enc.h"
#include "serv_user.h"
#include "serv_handle.h"

typedef struct thread_args
{
	pthread_t t_tid;
	int t_sckfd;
	/*每个用户的id关联一个子线程*/
	int t_id;
}THREAD_ARGS;



#endif // !IM_SERV_H
