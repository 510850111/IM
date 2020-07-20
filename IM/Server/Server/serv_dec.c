#include "im_serv.h"

int dec_reg_msg(char *buf, int n, REG_MSG **r)
{
	if (n != sizeof(REG_MSG))
	{
		printf("serv: reg msg len check failed!\n");
		return -1;
	}

	printf("serv: rev reg msg, len=%d\n", n);
	*r = (REG_MSG *)(buf + sizeof(MSG_HDR));

	return 0;
}

int dec_login_msg(char *buf, int n, LIG_MSG **l)
{
	if (n != sizeof(LIG_MSG))
	{
		printf("serv: login msg len check failed!\n");
		return -1;
	}

	*l = (LIG_MSG *)(buf + sizeof(MSG_HDR));
	(*l)->lg_id = ntohl((*l)->lg_id);
	printf("serv: rev login msg, len=%d, id=%d\n", n, (*l)->lg_id);
	return 0;
}

int dec_chat_msg(char *buf, int n, CHAT_MSG **c, int *rid)
{
	if(n <= sizeof(CHAT_MSG) || n > MAX_CHAT_MSG)
	{
		printf("serv: chat msg len check failed!\n");
		return -1;
	}
	printf("serv: recv chat msg, len=%d\n", n);
	*c = (CHAT_MSG *)(buf + sizeof(MSG_HDR));
	*rid = ntohl((*c)->ch_rid);

	return 0;
}

int dec_logout_msg(char *buf, int n, LOUT_MSG **l)
{
	if (n != sizeof(LOUT_MSG))
	{
		printf("serv: logout msg len check failed!\n");
		return -1;
	}
	printf("serv: rev logout msg, len=%d\n", n);
	*l = (LOUT_MSG *)(buf + sizeof(MSG_HDR));
	(*l)->lg_id = ntohl((*l)->lg_id);
	return 0;
}

int dec_fmgt_msg(char *buf, int n, FRND_OP **f, int **fid, int *cnt)
{
	int *p;
	if (n != sizeof(FRND_OP) &&
		((n - sizeof(FRND_OP)) % sizeof(int) != 0))
	{
		printf("serv: fmgt msg len check failed!\n");
		return -1;
	}
	printf("serv: rev fmgt msg, len=%d\n", n);
	*f = (FRND_OP *)(buf + sizeof(MSG_HDR));
	(*f)->f_id = ntohl((*f)->f_id);
	*cnt = (n - sizeof(FRND_OP)) / sizeof(int);
	p = (*f)->f_fids;
	*fid = p;
	for (int i = 0; i < *cnt; i++)
	{
		*p = ntohl(*p);
		p++;
	}

	return 0;
}

//int dec_client_msgs(char *buf, int n);