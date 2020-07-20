#pragma once
#ifndef CLIENT_HANDLE_H
#define CLIENT_HANDLE_H
#include "msg_type.h"

int handle_reg_resp(REG_RESP *r);

int handle_login_resp(LIG_RESP *l);

int handle_fmgt_resp(unsigned short stype, FRND_RESP *fr, FRND_ST *fs, short cnt);

int handle_chat_msg_or_resp(CHAT_MSG *c, CHAT_RESP *cr);

int handle_chat_resp(CHAT_RESP *c);

int handle_chat_msg(CHAT_MSG *c);

#endif // !CLIENT_HANDLE_H
