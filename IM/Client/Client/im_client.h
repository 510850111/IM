#pragma once
#ifndef IM_CLIENT_H
#define IM_CLIENT_H

#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#pragma comment(lib, "Ws2_32.lib")
//ftp://sourceware.org/pub/pthreads-win32/pthreads-w32-1-9-0-release.exe
#pragma comment(lib, "pthreadVC1.lib")

#include "client_config.h"
#include "client_user.h"
#include "cmsg_dec.h"
#include "cmsg_enc.h"
#include "cusr_cmd.h"
#include "list.h"
#include "msg_cipher.h"
#include "msg_type.h"
#include "client_handle.h"
#include "client_log.h"



#endif // !IM_CLIENT_H
