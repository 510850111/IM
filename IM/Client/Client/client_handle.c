#include "im_client.h"

int handle_reg_resp(REG_RESP *r)
{
	//printf("client: entry handle_reg_resp, myself.w_id = %d\n", myself.w_id = r->re_id);
	myself.w_id = r->re_id;
	printf("client: recv reg msg from server. uid=%d\n", r->re_id);

	if (r->re_id == -1)
	{
		fprintf(stderr, "%s register failed,reason:%s", myself.w_name, r->re_reason);
	}

	return 0;
}

int handle_login_resp(LIG_RESP *l)
{
	//printf("client: recv login resp from server.\n");

	if (l->lg_stat == LOGIN_OK)
	{
		myself.w_cstst = LOGIN_CSTAT;
		memset(myself.w_name, 0, MAXNAME_LEN);
		strncpy(myself.w_name, l->lg_name, MAXNAME_LEN - 1);

		return 0;
	}

	fprintf(stderr, "user %s  login failed, reason:%s\n", myself.w_name, l->lg_reason);

	return -1;
}

int handle_fmgt_resp(unsigned short stype, FRND_RESP *fr, FRND_ST *fs, short cnt)
{
	//printf("client: recv fmgt resp, stype=%d, cnt=%d\n", stype, cnt);

	/*
	#define	F_LREG		0x01
	#define F_ADD		0x02
	#define	F_DEL		0x03
	#define F_ALST		0x04
	#define F_STAT		0x05
	*/
	switch (stype)
	{
	case F_LREG:
	case F_ADD:
		user_list_add(fs, cnt);
		break;
	case F_DEL:
		user_list_del(fs, cnt);
		break;
	case F_ALST:
		user_list_output(fs, cnt);
		break;
	case F_STAT:
		user_list_update(fs, cnt);
	default:
		break;
	}

	return 0;
}

int handle_chat_msg_or_resp(CHAT_MSG *c, CHAT_RESP *cr)
{
	/*
	其他用户---> 自己的
	自己发送给别人的消息----->服务器反馈发送是否成功
	
	*/
	if (cr)
	{
		return handle_chat_resp(cr);
	}

	return handle_chat_msg(c);

}


int handle_chat_resp(CHAT_RESP *c)
{
	//printf("client: recv chat msg resp from server, c_stat=%d\n", c->c_stat);
	if (c->c_stat == SND_OK)
	{
		printf("client: chat msg send ok\n");
		return 0;
	}

	fprintf(stderr, "chat msg send error, reason: %s\n", c->c_reason);
	return -1;

}

int handle_chat_msg(CHAT_MSG *c)
{
	msg_decipher(c->ch_msg, CIPHER_KEY);
	printf("client: chat mes from %d : %s\n", c->ch_sid, c->ch_msg);

	client_log_save(c->ch_msg, c->ch_sid, c->ch_rid);

	return 0;
}