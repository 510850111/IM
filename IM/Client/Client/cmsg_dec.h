#pragma once
#ifndef CMSG_DEC_H
#define CMSG_DEC_H

int dec_server_resp(char *buf, int n);
int dec_reg_resp(char *buf, int n, REG_RESP **lr);
int dec_login_resp(char *buf, int n, LIG_RESP **l);
int dec_fmgt_resp(char *buf, int n, FRND_RESP **fr, FRND_ST **fs, short *cnt);
int dec_chat_msg_or_resp(char *buf, int n, CHAT_RESP **cr, CHAT_MSG **cm);
#endif // !CMSG_DEC_H
