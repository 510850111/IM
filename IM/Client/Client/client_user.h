#pragma once

#ifndef CLIENT_USER_H
#define CLIENT_USER_H

#include "msg_type.h"
#include "list.h"

typedef struct client_friend
{
	char c_name[MAXNAME_LEN];
	int c_id;

	struct list_head c_node;
	int c_stat;
}CLIENT_FRND;

#define ufrnd_for_each_entry(pos, head, member) \
	for(pos=(CLIENT_FRND *) list_entry((head)->next, CLIENT_FRND, member); \
		((&pos -> member)) != (head); \
		pos = (CLIENT_FRND *) list_entry((pos)->member.next, CLIENT_FRND,member))

#define INIT_CSTAT		0
#define LOGIN_CSTAT		1
#define MAX_MSG_SIZE	512
#define MSG_SNDING		0
#define MSG_RCVING		1

typedef struct whoami
{
	/*user id*/
	int w_id;
	/*user name */
	char w_name[MAXNAME_LEN];
	/*user passwd*/
	char w_pass[MAX_USERPASS_LEN];
	/*user socket fd*/
	int w_sockfd;
	/*user state : init, loggin*/
	int w_cstst;
	/*msg cnt*/
	int w_msgcnt;
	/*msg head*/
	struct list_head w_msghd;
	/*all friends user list head*/
	struct list_head w_flisthd;
	/*friend cnt*/
	int w_fndcnt;
	/*消息发送缓冲区*/
	char w_buf[MAX_MSG_SIZE];
	/*message state : sending, recving*/
	char w_mstat;
	/*expected msg tpye*/
	char w_msgtype;
}MYSELF;

extern struct whoami myself;

int init_user_struct(MYSELF *m);
int user_list_add(FRND_ST *fs, short cnt);
int user_list_del(FRND_ST *fs, short cnt);
int user_list_update(FRND_ST *fs, short cnt);
int user_list_output(FRND_ST *fs, short cnt);

#endif // !CLIENT_USER_H

