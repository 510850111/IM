#include "im_serv.h"

/*
服务器处理某个用户的子线程调用recv函数接收消息
调用handle_client_msgs对消息进行初步解析，判断消息长度是否合法，从消息头中获取消息类型及长度。
根据不同的消息类型，调用对应的函数解析。
若消息解析成功，调用对应的消息处理函数。
处理完成之后，再调用recv函数收取下一条消息进行处理。

*/

int serv_snd_msg(int *usr_id, int fd, char *buf, int n)
{
	if (send(fd, buf, n, 0) != n)
	{
		printf("serv: send msg error, user exit...\n");
		if (*usr_id != 0)
		{
			usr_logout_exit(*usr_id);
		}
		return -2;
	}
	printf("serv: serv_snd_msg, n=%d\n", n);
	return n;
}

int handle_reg_msg(int *usr_id, int fd, REG_MSG *r)
{
	char buf[MAX_MSG_SIZE], err[MAX_ERR_LEN];
	int uid = get_next_uid(); //1001
	int n, ret;
	IM_USER *u = &all_users[uid - CONF_LOW_ID];
	memset(buf, 0, MAX_MSG_SIZE);
	memset(err, 0, MAX_ERR_LEN);
	if (reg_user_num >= MAX_USER_NUM)
	{
		printf("serv: reg failed, maximum user reached!\n");
		strncpy(err, "maximum user reached!", MAX_ERR_LEN - 1);
		enc_reg_resp(buf, &n, -1, err);

		return serv_snd_msg(usr_id, fd, buf, n);
	}

	enc_reg_resp(buf, &n, uid, NULL);
	ret = serv_snd_msg(usr_id, fd, buf, n);
	if (ret > 0)
	{
		memset(u->u_name, 0, MAXNAME_LEN);
		strncpy(u->u_name, r->r_name, MAXNAME_LEN - 1);

		memset(u->u_pass, 0, MAX_USERPASS_LEN);
		strncpy(u->u_pass, r->r_passwd, MAX_USERPASS_LEN - 1);

		u->u_id = uid;
		u->u_stat = USER_REG;
		u->u_sckfd = fd;

		reg_user_num_inc();
	}

	printf("serv: send reg resp, n=%d\n", n);
	return ret;
}

int handle_login_msg(int *usr_id, int fd, LIG_MSG *l)
{
	int uid = l->lg_id;
	IM_USER *u = &all_users[uid - CONF_LOW_ID];
	char buf[MAX_MSG_SIZE], err[MAX_ERR_LEN];
	int n, ret;
	int cmp = strcmp(u->u_pass, l->lg_pass);
	printf("serv: hanlde login msg from client.\n");
	memset(buf, 0, MAX_MSG_SIZE);
	if ((u->u_stat & USER_REG) &&
		!(u->u_stat & USER_LOGIN) &&
		!cmp)
	{
		enc_login_resp(buf, &n, LOGIN_OK, u->u_name, NULL);
	}
	ret = serv_snd_msg(uid, fd, buf, n);
	if (ret > 0)
	{
		u->u_sckfd = fd;
		u->u_stat |= USER_LOGIN;

		*usr_id = uid;
	}
	printf("serv: send login resp(login ok), n=%d\n", n);
	return ret;
}


int handle_chat_msg(int *usr_id, int fd, char *buf, int n, CHAT_MSG *c, int rid)
{
	IM_USER *u = &all_users[rid - CONF_LOW_ID];
	int ret;
	char rbuf[MAX_MSG_SIZE];
	MSG_HDR *h;

	memset(rbuf, 0, MAX_MSG_SIZE);
	if (!(u->u_stat & USER_LOGIN) ||
		u->u_sckfd < 0)
	{
		ret = -1;
		printf("serv: char msg snd failed, friend not login!\n");
		enc_chat_resp(rbuf, &n, SND_ERR,"friend not login!");
	}
	else
	{
		h = (MSG_HDR *)buf;
		h->msg_type = htons(ntohs(h->msg_type) | SERVER_FORWARD_FLAG);

		ret = serv_snd_msg(usr_id, u->u_sckfd, buf, n);
		if (ret <= 0)
		{
			usr_logout_exit(rid);
			enc_chat_resp(rbuf, &n, SND_ERR, "chat msg snd err!");
		}
		else
		{
			enc_chat_resp(rbuf, &n, SND_OK, NULL);
		}

	}
	printf("serv: forwaing chat resp, n=%d", n);
	return serv_snd_msg(usr_id, fd, rbuf, n);
}


int handle_logout_msg(int *usr_id, LOUT_MSG *l)
{
	int uid = l->lg_id;
	IM_USER *u = &all_users[uid - CONF_LOW_ID];

	/*
	0000 0000 0000 0001
	1111 1111 1111 1110

	*/

	u->u_stat &= ~USER_LOGIN;
	return 0;
}

int handle_fmgt_msg(int *usr_id, int fd, unsigned char stype, FRND_OP *f, int *fid, int cnt)
{
	int uid = f->f_id;
	IM_USER *u = &all_users[uid - CONF_LOW_ID];

	char buf[MAX_MSG_SIZE];
	int friend_cnt = u->u_frndcnt;
	int n;
	FRND_ST *fs;
	USR_FRND *uf;
	FRND_ST fnds;
	FRND_ST users_ret[MAX_USER_SIZE];
	int user_cnt= MAX_USER_SIZE;
	int ret;
	if (cnt != 1 && cnt != 0)
	{
		printf("serv: fmgt msg, invalid cnt=%d\n", cnt);
		return -1;
	}
	/*
	#define F_LREG		0x01
	#define F_ADD		0x02
	#define	F_DEL		0x03
	#define F_ALST		0x04
	#define F_STAT		0x05
	*/
	if (stype == F_LREG)
	{
		printf("serv: user lreg msg\n");
		fs = malloc(friend_cnt * sizeof(FRND_ST));
		if (fs == NULL)
		{
			printf("serv: memory allocation failed for frnd_st\n");
			return -1;
		}
		usr_fill_frndst(fs, friend_cnt, u);
		enc_fmgt_resp(buf, &n, F_LREG, OP_ALL_OK, fs, friend_cnt);
	}
	else if (stype == F_ADD)
	{
		printf("serv: user add msg\n");
		//rintf("fid=%d\n", *fid);
		ret = usr_friend_add(u, *fid);
		printf("out of usr_friend_add\n");
		if (ret < 0)
		{
			printf("serv: user alread exist in friend list.\n");
			enc_fmgt_resp(buf, &n, F_ADD, OP_ALL_FAIL, NULL, 0);
		}
		else
		{
			memset(&fnds, 0, sizeof(fnds));
			u = &all_users[*fid - CONF_LOW_ID];
			strncpy(fnds.fs_name, u->u_name, MAXNAME_LEN - 1);
			fnds.fs_id = u->u_id;
			fnds.fs_stat = u->u_stat;
			u->u_frndcnt++;
			printf("serv: entry enc_fmgt_resp\n");
			enc_fmgt_resp(buf, &n, F_ADD, OP_ALL_OK, &fnds, 1);
		}

	}
	else if (stype == F_DEL)
	{
		
		printf("serv: user del msg\n");
		ret = usr_friend_del(u, *fid);
		printf("serv: usr_friend_del return %d\n", ret);
		if (ret < 0)
		{
			printf("serv: user NOT exist in friend list.\n");
			enc_fmgt_resp(buf, &n, F_DEL, OP_ALL_FAIL, NULL, 0);
		}
		else
		{
			memset(&fnds, 0, sizeof(fnds));
			u = &all_users[*fid - CONF_LOW_ID];
			strncpy(fnds.fs_name, u->u_name, MAXNAME_LEN - 1);
			fnds.fs_id = u->u_id;
			fnds.fs_stat = u->u_stat;
			u->u_frndcnt--;
			printf("serv: entry enc_fmgt_resp[F_DEL] ALL_OK\n");
			enc_fmgt_resp(buf, &n, F_DEL, OP_ALL_OK, &fnds, 1);
		}
	}
	else if (stype == F_ALST)
	{
		printf("serv: user ALIST msg\n");
		get_online_users(users_ret, &user_cnt);
		enc_fmgt_resp(buf, &n, F_ALST, OP_ALL_OK,users_ret, user_cnt);
	}
	else if (stype == F_STAT)
	{
		printf("serv: user STAT msg\n");
		uf = usr_friend_find(u, *fid);
		if (uf == NULL)
		{
			enc_fmgt_resp(buf, &n, F_STAT, OP_ALL_FAIL, NULL, 0);
		}
		else
		{
			memset(&fnds, 0, sizeof(fnds));
			u = &all_users[*fid - CONF_LOW_ID];
			strncpy(fnds.fs_name, u->u_name, MAXNAME_LEN - 1);
			fnds.fs_id = u->u_id;
			fnds.fs_stat = u->u_stat;

			enc_fmgt_resp(buf, &n, F_STAT, OP_ALL_OK, &fnds, 1);
		}
	}

	printf("serv: sending fmgt resp, n=%d\n", n);
	return serv_snd_msg(usr_id, fd, buf, n);
}


int handle_client_msgs(int *uid, int fd, char *buf, int n)
{
	unsigned char type, stype;
	REG_MSG		*r;
	LIG_MSG		*li;
	CHAT_MSG	*c;
	LOUT_MSG	*lo;
	FRND_OP		*f;

	MSG_HDR *h = (MSG_HDR *)buf;

	int *fid, cnt, rid;

	short m_type, m_len;
	m_type = ntohs(h->msg_type);
	m_len = ntohs(h->msg_len);

	if (n != sizeof(MSG_HDR) + m_len)
	{
		printf("serv: msg len sanity check failed!\n");
		return -1;
	}

	/*
	#define MSG_FRNDMGT	0x03
	#define F_ADD		0x02

	0000 0011 0000 0010
	0000 0000 0000 0011  //MSG_FRNDMGT
	1111 1111 1111 1111

	0000 0011 0000 0010
	1111 1111 1111 1111

	0000 0011 0000 0010


	*/

	type = (m_type >> 8) & 0xff ;   //0000 0011
	stype = m_type & 0xff;			//0000 0010

	switch (type)
	{
	case MSG_REG:
		if (dec_reg_msg(buf, m_len, &r) >= 0)
		{
			return handle_reg_msg(uid,fd,r);
		}
		break;

	case MSG_LOGIN:
		if (dec_login_msg(buf, m_len, &li) >= 0)
		{
			return handle_login_msg(uid, fd, li);
		}
		break;

	case MSG_FRNDMGT:
		if (dec_fmgt_msg(buf, m_len, &f, &fid, &cnt) >= 0)
		{
			return handle_fmgt_msg(uid, fd, stype, f, fid, cnt);
		}
		break;

	case MSG_CHAT:
		if (dec_chat_msg(buf, m_len, &c, &rid) >= 0)
		{
			return handle_chat_msg(uid, fd, buf, n, c, rid);
		}
		break;

	case MSG_LOGOUT:
		if (dec_logout_msg(buf, m_len, &lo) >= 0)
		{
			return handle_logout_msg(uid, lo);
		}
		break;

	default:
		return -1;
	}

	return -1;
}