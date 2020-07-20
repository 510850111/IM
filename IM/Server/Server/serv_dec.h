#pragma once
#ifndef SERV_DEC_H
#define SERV_DEC_H

#include "im_serv.h"

int dec_reg_msg(char *buf, int n, REG_MSG **r);
int dec_login_msg(char *buf, int n,	LIG_MSG **l);
int dec_chat_msg(char *buf, int n, CHAT_MSG **c, int *rid);
int dec_logout_msg(char *buf, int n, LOUT_MSG **l);
int dec_fmgt_msg(char *buf, int n,FRND_OP **f, int **fid, int *cnt);
int dec_client_msgs(char *buf, int n);

#endif // !SERV_DEC_H

