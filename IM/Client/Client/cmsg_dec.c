#include "im_client.h"

/*
对收到的消息进行统一处理
处理流程包含获取消息的类型、子类型、消息长度
判断消息长度字段是否合法

switch对不同的消息进行不同的处理
dec_reg_resp();

*/

int dec_server_resp(char *buf, int n)
{
	unsigned char type;
	REG_RESP *r;
	LIG_RESP *l;
	FRND_RESP *fr;
	FRND_ST	*fs;
	CHAT_RESP *c;
	CHAT_MSG *cm;

	short cnt;
	MSG_HDR *h = (MSG_HDR *)buf;
	short m_type, m_len;
	unsigned char stype;

	m_type = ntohs(h->msg_type);
	m_len = ntohs(h->msg_len);

	stype = m_type & 0xff;

	if (n != sizeof(MSG_HDR) + m_len)
	{
		printf("client: resp msg len check error!\n");
		return -1;
	}

	/*
	0x03
	0000 0011

	0x02 
	0000 0010

	short 2 byte

	0000 0011 0000 0010

	0000 0000 0000 0011
	0xff
	1111 1111 1111 1111

	0x03

	0xff =255
	0x7f = 127
	*/
	type = (m_type >> 8) & 0xff;
	

	switch (type & 0x7f)
	{
	case MSG_REG:
		if (dec_reg_resp(buf, m_len, &r) >= 0)
		{
			//printf("client: dec_reg_resp \n");
			handle_reg_resp(r);
		}
		break;
	case MSG_LOGIN:
		if (dec_login_resp(buf, m_len, &l) >= 0)
		{
			//printf("client: dec_login_resp \n");
			handle_login_resp(l);
		}
		break;
	case MSG_FRNDMGT:
		if (dec_fmgt_resp(buf, m_len, &fr, &fs, &cnt) >= 0)
		{
			//printf("client: dec_fmgt_resp \n");
			handle_fmgt_resp(stype, fr, fs, cnt);
		}
		break;
	case MSG_CHAT:
		if (dec_chat_msg_or_resp(buf, m_len, &c, &cm) >= 0)
		{
			//printf("client: dec_chat_msg_or_resp \n");
			handle_chat_msg_or_resp(cm, c);
		}
		break;


	default:
		break;
	}
	//printf("client: out of dec_server_resp \n");
	return 0;
}

int dec_reg_resp(char *buf, int n, REG_RESP **lr)
{
	MSG_HDR *h = (MSG_HDR *)buf;
	REG_RESP *r = (REG_RESP *)(h + 1);

	if (n > sizeof(REG_RESP))
	{

		if (n > sizeof(REG_RESP) + MAX_ERR_LEN ||
			n != sizeof(REG_RESP) + strlen(r->re_reason) + 1)
		{
			printf("client: reg resp ,invalid msg len=%d\n", n);
			return -1;
		}
	}
	else if (n != sizeof(REG_RESP))
	{
		printf("client: reg resp2,invalid msg len=%d\n", n);
		return -1;
	}

	/*
	0000 0000 1111 1111	大端
	1111 1111 0000 0000 小端
	*/
	r->re_id = ntohl(r->re_id);
	*lr = r;
	return 0;
	
}


int dec_login_resp(char *buf, int n, LIG_RESP **l)
{
	MSG_HDR *h = (MSG_HDR *)buf;
	*l = (LIG_RESP *)(h + 1);

	if (n > sizeof(LIG_RESP))
	{
		if (n > sizeof(LIG_RESP) + MAX_ERR_LEN ||
			n != sizeof(LIG_RESP) + strlen((*l)->lg_reason) + 1)
		{
			printf("client: login resp ,invalid msg len=%d\n", n);
			return -1;
		}
		else if (n != sizeof(LIG_RESP))
		{
			printf("client: login resp2,invalid msg len=%d\n", n);
			return -1;
		}
	}

	(*l)->lg_stat = ntohl((*l)->lg_stat);

	return 0;
}

int dec_chat_msg_or_resp(char *buf, int n, CHAT_RESP **cr, CHAT_MSG **cm)
{
	MSG_HDR *h = (MSG_HDR *)buf;
	short msg_type = ntohs(h->msg_type);

	/*
	0000 0000 0000 0001
	1000 0000 0000 0000

	0x04 
	0000 0000 0000 0100

	MSG_CHAT X
	0000 0100 0000 0000
	1000 0000 0000 0000


	server :用户发送消息 MSG_CHAT | SERVER_FORWARD_FLAG
	0000 0100 0000 0000 
	1000 0000 0000 0000
	实际收到的标志位消息↓
	1000 0100 0000 0000



	server :服务器回复消息 MSG_CHAT 
	实际收到的标志位消息↓
	MSG_CHAT 
	0000 0100 0000 0000
	1000 0000 0000 0000
	*/

	/*msg_type：1000 0100 0000 0000*/
	
	if (msg_type & SERVER_FORWARD_FLAG)
	{
		printf("client: real chat msg,n=%d\n", n);
		*cr = NULL;
		*cm = (CHAT_MSG *)(h + 1);
	}
	else
	{
		printf("client: chat resp msg, n=%d\n", n);
		*cm = NULL;
		*cr = (CHAT_RESP *)(h + 1);
	}

	if (*cr)
	{
		if (n > sizeof(CHAT_RESP))
		{
			if (n > sizeof(CHAT_RESP) + MAX_ERR_LEN ||
				n != sizeof(CHAT_RESP) + strlen((*cr)->c_reason) + 1)
			{
				printf("client: chat resp ,invalid msg len=%d\n", n);
				return -1;
			}
			else if (n != sizeof(CHAT_RESP))
			{
				printf("client: chat resp2,invalid msg len=%d\n", n);
				return -1;
			}
		}
	}
	else
	{
		if (n > MAX_MSG_SIZE || n < sizeof(CHAT_MSG))
		{
			printf("client: chat msg ,invalid msg len=%d\n", n);
			return -1;
		}
		(*cm)->ch_sid = ntohl((*cm)->ch_sid);
		(*cm)->ch_rid = ntohl((*cm)->ch_rid);
	}

	return 0;
}

int dec_fmgt_resp(char *buf, int n, FRND_RESP **fr, FRND_ST **fs, short *cnt)
{
	MSG_HDR *h = (MSG_HDR *)buf;

	FRND_RESP *f = (FRND_RESP *)(h + 1);
	FRND_ST *ps = (FRND_ST *)(f + 1);

	*cnt = (n - sizeof(FRND_RESP)) / (sizeof(FRND_ST));

	if (n > sizeof(FRND_RESP))
	{
		
		if ((n - sizeof(FRND_RESP)) % sizeof(FRND_ST) != 0 ||
			*cnt != ntohs(f->fre_num))
		{
			printf("client: fmgt resp ,invalid len=%d, cnt=%d\n", n, *cnt);
			return -1;
		}
	}
	else if (n != sizeof(REG_RESP))
	{
		printf("client: fmgt resp2 ,invalid len=%d\n", n);
		return -1;
	}

	f->fre_stat = ntohs(f->fre_stat);
	*fr = f;
	*fs = ps;

	for (int i = 0; i < *cnt; i++, ps++)
	{
		ps->fs_id = ntohl(ps->fs_id);
		ps->fs_stat = ntohs(ps->fs_stat);
		printf("serv: user id=%d, stat=%d\n", ps->fs_id, ps->fs_stat);
	}
	return 0;
}
