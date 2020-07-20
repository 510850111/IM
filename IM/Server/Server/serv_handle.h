#pragma once
#ifndef SERV_HANDLE_H
#define SERV_HANDLE_H

#include "im_serv.h"

int serv_snd_msg(int *usr_id, int fd, char *buf, int n);
int handle_reg_msg(int *usr_id, int fd, REG_MSG *r);
int handle_login_msg(int *usr_id, int fd, LIG_MSG *l);
int handle_chat_msg(int *usr_id, int fd,char *buf, int n, CHAT_MSG *c, int rid);
int handle_logout_msg(int *usr_id, LOUT_MSG *l);
int handle_fmgt_msg(int *usr_id, int fd, unsigned char stype, FRND_OP *f, int *fid, int cnt);
int handle_client_msgs(int *uid, int fd, char *buf, int n);

#endif // !SERV_HANDLE_H
