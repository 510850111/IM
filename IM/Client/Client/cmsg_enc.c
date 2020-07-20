#include "im_client.h"

int init_reg_stuct(REG_MSG *r, MYSELF *m)
{
	memset(r, 0, sizeof(*r));
	memcpy(r->r_name, m->w_name, MAXNAME_LEN);
	strncpy(r->r_passwd, m->w_pass, MAX_USERPASS_LEN);

	return sizeof(REG_MSG);
}

int enc_reg_msg(char *buf, int *n, unsigned char stype)
{
	*n = init_reg_stuct((REG_MSG *)(((MSG_HDR *)buf)->msg_data), &myself);
	init_msg_hdr((MSG_HDR *)(buf), MSG_REG, stype, sizeof(REG_MSG));
	*n += sizeof(MSG_HDR);

	return *n;

}
void init_msg_hdr(MSG_HDR *h, unsigned char type, unsigned char stype, int n)
{
	h->msg_type = htons((type << 8) | stype);
	h->msg_len = htons(n);
}

int init_login_struct(LIG_MSG *l, MYSELF *m)
{
	memset(l, 0, sizeof(*l));
	l->lg_id = htonl(m->w_id);
	strcpy(l->lg_pass, m->w_pass, MAX_USERPASS_LEN - 1);
	
	return sizeof(LIG_MSG);

}

/*
封装登录消息
@buf: 消息缓冲区
@n: 消息长度
@stype: 消息子类型
*/
int enc_login_msg(char *buf, int *n, unsigned char stype)
{
	*n = init_login_struct((LIG_MSG *)(((MSG_HDR *)buf)->msg_data), &myself);
	init_msg_hdr((MSG_HDR *) buf, MSG_LOGIN, stype, sizeof(LIG_MSG));

	*n += sizeof(MSG_HDR);

	return *n;
}

int init_chat_struct(CHAT_MSG *c, MYSELF *m, char *msg, int rid)
{
	memset(c, 0, sizeof(*c));
	c->ch_sid = htonl(m->w_id);
	c->ch_rid = htonl(rid);

	/*加密的消息*/
	msg_encipher(msg, CIPHER_KEY);
	strcpy(c->ch_msg, msg);

	return sizeof(*c) + strlen(msg) + 1;
}

int enc_chat_msg(char *buf, int *n, unsigned char stype, int rid, char *msg)
{
	*n = init_chat_struct((CHAT_MSG *)(((MSG_HDR *)buf)->msg_data), &myself, msg, rid);
	init_msg_hdr((MSG_HDR *)buf, MSG_CHAT, stype, *n);
	*n += sizeof(MSG_HDR);

	return *n;
}

int init_logout_struct(LOUT_MSG *c, MYSELF *m)
{
	memset(c, 0, sizeof(*c));
	c->lg_id = htonl(m->w_id);
	strncpy(c->lg_pass, m->w_pass, MAX_USERPASS_LEN - 1);
	
	return sizeof(LOUT_MSG);
}

int enc_logout_msg(char *buf, int *n, unsigned char stype)
{
	init_logout_struct((LOUT_MSG *)(((MSG_HDR *)buf)->msg_data), &myself);
	init_msg_hdr((MSG_HDR *)buf, MSG_LOGOUT, stype, sizeof(LOUT_MSG));
	*n = sizeof(LOUT_MSG) + sizeof(MSG_HDR);

	return *n;
}

int init_fmgt_struct(FRND_OP *f, MYSELF *m, unsigned char stype, int *id, int cnt)
{
	/*
	#define F_LREG		0x01
	#define F_ADD		0x02
	#define	F_DEL		0x03
	#define F_ALST		0x04
	#define F_STAT		0x05
	*/
	int *p = f->f_fids;

	f->f_id = htonl(m->w_id);
	for (int i = 0; i < cnt; i++, p++, id++)
	{
		*p = htonl(*id);
	}

	return sizeof(FRND_OP) + sizeof(int)*cnt;
}

int enc_fmgt_msg(char *buf, int *n, unsigned char stype, int *id, int cnt)
{
	
	*n = init_fmgt_struct((FRND_OP *)(((MSG_HDR *)buf)->msg_data), &myself, stype, id, cnt);
	init_msg_hdr((MSG_HDR *)buf, MSG_FRNDMGT, stype, *n);
	*n += sizeof(MSG_HDR);

	return *n;
}
