#include "im_serv.h"

void init_msg_hdr(MSG_HDR *h, int n, unsigned char type, unsigned char stype)
{
	/*
	大端字节序
	0000 0000 0000 0000		//short
	0000 0001				// uchar type
	0000 0010				// uchar stype

	0000 0001 0000 0000		//(u_short)type<<8
	0000 0000 0000 0010		//(u_stype)
	按位或
	0000 0001 0000 0010

	*/
	h->msg_type = htons((type << 8) | stype);
	h->msg_len = htons(n);
}

int init_reg_struct(REG_RESP *r, int id, char *reason)
{
	memset(r, 0, sizeof(*r));

	r->re_id = htonl(id);
	if (id == -1)
	{
		strncpy(r->re_reason, reason, MAX_ERR_LEN - 1);

		return sizeof(REG_RESP) + strlen(reason) + 1;
	}
	return sizeof(REG_RESP);
}

int enc_reg_resp(char *buf, int *n, int id, char *reason)
{


	*n = init_reg_struct((REG_RESP *)((MSG_HDR *)buf)->msg_data, id, reason);

	init_msg_hdr((MSG_HDR *)buf, *n, MSG_REG, 0);
	*n += sizeof(MSG_HDR);
	
	return *n;
}

int init_login_struct(LIG_RESP *l, int stat, char *name, char *reason)
{
	memset(l, 0, sizeof(*l));
	l->lg_stat = htonl(stat);
	if (stat == LOGIN_ERR)
	{
		strncpy(l->lg_reason, reason, MAX_ERR_LEN - 1);
		return sizeof(LIG_RESP) + strlen(reason) + 1;
	}
	if (name)
	{
		strncpy(l->lg_name, name, MAXNAME_LEN - 1);
	}

	return sizeof(LIG_RESP);

}

int enc_login_resp(char *buf, int *n, int stat, char *name, char *resaon)
{
	*n = init_login_struct((LIG_RESP *)(((MSG_HDR *)buf)->msg_data),stat, name, resaon);
	init_msg_hdr((MSG_HDR *)buf, *n, MSG_LOGIN, 0);
	*n += sizeof(MSG_HDR);
	printf("serv: enc_login_resp, n=%d\n", n);
	return *n;
}
int init_fmgt_struct(FRND_RESP *f, unsigned char stype, short stat, FRND_ST *fs, short cnt)
{
	int len = sizeof(FRND_RESP);
	FRND_ST *p = (FRND_ST *)((char *)f + len);
	f->fre_num = htons(cnt);
	f->fre_stat = htons(stat);

	/*
	#define F_LREG		0x01
	#define F_ADD		0x02
	#define	F_DEL		0x03
	#define F_ALST		0x04
	#define F_STAT		0x05
	*/
	for (int i = 0; i < cnt; i++, p++, fs++)
	{
		p->fs_id = htonl(fs->fs_id);
		p->fs_stat = htonl(fs->fs_stat);
		memset(p->fs_name, 0, MAXNAME_LEN);
		strncpy(p->fs_name, fs->fs_name, MAXNAME_LEN);

		len += sizeof(*p);
	}

	return len;
}
int enc_fmgt_resp(char *buf, int *n, unsigned char stype, short stat, FRND_ST *fs, short cnt)
{
	*n = init_fmgt_struct((FRND_RESP *)(((MSG_HDR *)buf)->msg_data), stype, stat, fs, cnt);
	init_msg_hdr((MSG_HDR *)buf, *n, MSG_FRNDMGT, 0);
	*n += sizeof(MSG_HDR);

	return *n;
}

int init_chat_struct(CHAT_RESP *c, int stat, char *reason)
{
	memset(c, 0, sizeof(*c));
	c->c_stat = htonl(stat);
	if (stat == SND_ERR)
	{
		strncpy(c->c_reason, reason, MAX_ERR_LEN - 1);
		return sizeof(CHAT_RESP) + strlen(reason) + 1;
	}
	return sizeof(CHAT_RESP);
}

int enc_chat_resp(char *buf, int *n, int stat, char *reason)
{
	/*int init_login_struct(LIG_RESP *l, int stat, char *name, char *reason);*/
	*n = init_chat_struct((CHAT_RESP *)(((MSG_HDR *)buf)->msg_data), stat, reason);
	init_msg_hdr((MSG_HDR *)buf, *n, MSG_FRNDMGT, 0);
	*n += sizeof(MSG_HDR);

	return *n;
}