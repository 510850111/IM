#pragma once
#ifndef SERV_USER_H
#define SERV_USER_H

#include "im_serv.h"

#define USER_REG	(0x1 << 0)
#define USER_LOGIN	(0x1 << 1)

typedef struct im_user
{
	char u_name[MAXNAME_LEN];
	char u_pass[MAX_USERPASS_LEN];
	int u_id;
	int u_stat;
	int u_sckfd;

	struct list_head u_frndhd;
	int u_frndcnt;
}IM_USER;


typedef struct usr_frnd
{
	int f_id;
	struct list_head f_node;
}USR_FRND;

extern IM_USER all_users[MAX_USER_NUM];
extern int reg_user_num;


#define uflist_for_each_entry(pos, head, member)	\
	for(pos=(USR_FRND *)list_entry((head)->next, USR_FRND, member); \
		&pos->member != (head);	\
		pos = (USR_FRND *)list_entry(pos->member.next, USR_FRND, member))

int get_next_uid();
void reg_user_num_inc();
int init_all_usr_struct(int argc, char **argv);
void usr_fill_frndst(FRND_ST *fs,int cnt,IM_USER *u);
void get_online_users(FRND_ST *fs, int *cnt);
USR_FRND *usr_friend_find(IM_USER *u, int uid);
int usr_friend_add(IM_USER *u,int uid);
int usr_friend_del(IM_USER *u, int uid);
void usr_logout_exit(int uid);

#endif // !SERV_USER_H
