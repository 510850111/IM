#include "im_client.h"

/*

流程：
1. 调用fgest函数从标准输入中获取用户输入的命令，并对该命令进行预处理。
2. strtok_s函数将用户命令的第一个字段解析出来（命令关键字解析）
3. 用一个多分支的if-else语句判断命令的类型，根据命令调用不同的命令处理函数
4. 在命令处理函数中调用cmdline_proc函数对用户的命令关键字后的各字段进行提取
5. 若用户输入的命令字段合法，对字段内容进行合法性验证。
6. 根据命令类型判断，该命令是网络命令还是本地命令。
	若为网络命令，则对对应的消息进行封装，调用client_send函数把封装的消息发完服务器。
	若为本地命令，则调用相应的程序进行处理，然后命令处理完毕，再转至第1步，继续等待用户输入的下一个命令并处理。
*/

/*对命令的各个字段进行分解
@n_tok: 去除了命令关键字的命令行字符串指针
@p: 各个字段的指针数组
@cnt: 数组的成员个数
@strict: 格式是否有严格的格式标志 【1|0】
*/
int cmdline_proc(char *n_tok, char **p, int cnt, int strict)
{
	int n;
	char *token = NULL, seps[] = " ";
	if (strict)
	{
		n = cnt;
	}
	else
	{
		n = cnt - 1;
	}

	for (int i = 0; i < n; i++)
	{
		token = strtok_s(NULL, seps, &n_tok);
		p[i] = token;
		/*用户提供的命令参数不够*/
		if (i != n - 1 && !p[i])
		{
			return -1;
		}
	}

	if (strict && p[cnt - 1])
	{
		return -1;
	}

	if (!strict)
	{
		p[cnt - 1] = n_tok;
		if (!n_tok)
		{
			return -1;
		}
	}

	return 0;
}

int client_send(char *buf, int n)
{
	int len;
	//printf("client_send(),myself.w_sockfd=%d\n", myself.w_sockfd);
	len = send(myself.w_sockfd, buf, n, 0);
	if (len != n)
	{
		closesocket(myself.w_sockfd);
		init_user_struct(&myself);
	}

	return  len;
}

int reg_cmd_proc(char *n_tok)
{
	int n,len;
	char *p[REG_CMD_FLDS];

	if (cmdline_proc(n_tok, p, REG_CMD_FLDS, 1) < 0)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, REG_C_USAGE);
		return -1;
	}

	if (strlen(p[0]) > MAXNAME_LEN - 1)
	{
		fprintf(stderr, "name too long, max=%d characters allowed\n%s", MAXNAME_LEN - 1, REG_C_USAGE);
		return -1;
	}

	if (strcmp(p[1], p[2]))
	{
		fprintf(stderr, "password must match!\n%s", REG_C_USAGE);
	}

	if (strlen(p[1]) > MAX_USERPASS_LEN - 1)
	{
		fprintf(stderr, "password too long, max=%d characters allowed\n%s", MAXNAME_LEN - 1, REG_C_USAGE);
		return -1;
	}

	strcpy(myself.w_name, p[0]);
	strcpy(myself.w_pass, p[1]);

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	enc_reg_msg(myself.w_buf, &n, 0);
	//printf("enc_reg_msg ok, n=%d\n", n);
	len = client_send(myself.w_buf, n);
	//printf("client_send ok, len=%d\n", len);
	myself.w_mstat = MSG_RCVING;
	//printf("reg_cmd_proc ok\n");

	return 0;
}

int login_cmd_proc(char *n_tok)
{
	int id,n;
	char *p[LOGIN_CMD_FLDS];
	if (cmdline_proc(n_tok, p, LOGIN_CMD_FLDS, 1) == -1
		|| !(id = atoi(p[0]))
		|| strlen(p[1]) > MAX_USERPASS_LEN - 1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, LIN_C_USAGE);
		return -1;
	}

	myself.w_id = id;
	memset(myself.w_pass, 0, MAX_USERPASS_LEN);
	strncpy(myself.w_pass, p[1], MAX_USERPASS_LEN - 1);

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	enc_login_msg(myself.w_buf, &n, 0);
	client_send(myself.w_buf, n);

	myself.w_mstat = MSG_RCVING;
	
	return 0;
}

int mesg_cmd_proc(char *n_tok)
{
	char *p[MESG_CMD_FLDS];
	int from, to;
	FILE *fp;

	if(cmdline_proc(n_tok, p, MESG_CMD_FLDS, 1) == -1
		|| !(from=atoi(p[0]))
		|| !(to = atoi(p[1])))
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, MESG_C_USAGE);
		return -1;
	}

	fp = fopen(CLIENT_LOG_FILE, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "open file \"%s\" failed!\n", CLIENT_LOG_FILE);
		return -1;
	}

	client_log_export(fp, stdout, from, to);

	fclose(fp);
	return 0;
}

int flist_cmd_proc(char *n_tok)
{
	int n;
	char *p[FLIST_CMD_FLDS];
	if (cmdline_proc(n_tok, p, FLIST_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, FLIST_C_USAGE);
		return -1;
	}

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	enc_fmgt_msg(myself.w_buf, &n, F_LREG, NULL, 0);

	client_send(myself.w_buf, n);

	myself.w_mstat = MSG_RCVING;
	return 0;
}

int add_cmd_proc(char *n_tok)
{
	int n, uid;
	char *p[ADD_CMD_FLDS];

	if (cmdline_proc(n_tok, p, ADD_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, FADD_C_USAGE);
		return -1;
	}

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	uid = atoi(p[0]);

	enc_fmgt_msg(myself.w_buf, &n, F_ADD, &uid, 1);
	client_send(myself.w_buf, n);
	myself.w_mstat = MSG_RCVING;

	return 0;
}

int del_cmd_proc(char *n_tok)
{
	int n, uid;
	char *p[DEL_CMD_FLDS];

	if (cmdline_proc(n_tok, p, DEL_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, FDEL_C_USAGE);
		return -1;
	}

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	uid = atoi(p[0]);

	enc_fmgt_msg(myself.w_buf, &n, F_DEL, &uid, 1);
	client_send(myself.w_buf, n);
	myself.w_mstat = MSG_RCVING;

	return 0;
}

int alist_cmd_proc(char *n_tok)
{
	int n;
	char *p[ALIST_CMD_FLDS];

	if (cmdline_proc(n_tok, p, ALIST_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, ALIST_C_USAGE);
		return -1;
	}

	memset(myself.w_buf, 0, MAX_MSG_SIZE);

	enc_fmgt_msg(myself.w_buf, &n, F_ALST, NULL, 0);
	client_send(myself.w_buf, n);
	myself.w_mstat = MSG_RCVING;

return 0;
}


int fstat_cmd_proc(char *n_tok)
{
	int n, uid;
	char *p[FSTST_CMD_FLDS];

	if (cmdline_proc(n_tok, p, FSTST_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, FSTAT_C_USAGE);
		return -1;
	}

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	uid = atoi(p[0]);

	enc_fmgt_msg(myself.w_buf, &n, F_STAT, &uid, 1);
	client_send(myself.w_buf, n);
	myself.w_mstat = MSG_RCVING;

	return 0;
}

int chat_cmd_proc(char *n_tok)
{
	int n, rid;
	char *p[CHAT_CMD_FLDS];

	if (cmdline_proc(n_tok, p, CHAT_CMD_FLDS, 0) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, CHAT_C_USAGE);
		return -1;
	}

	memset(myself.w_buf, 0, MAX_MSG_SIZE);
	rid = atoi(p[0]);
	if (!rid)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, CHAT_C_USAGE);
		return -1;
	}

	enc_chat_msg(myself.w_buf, &n, 0, rid, p[1]);
	client_send(myself.w_buf, n);
	myself.w_mstat = MSG_RCVING;

	return 0;
}

int logout_cmd_proc(char *n_tok)
{
	int n;
	char *p[LOGOUT_CMD_FLDS];
	if (cmdline_proc(n_tok, p, LOGOUT_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, EXIT_C_USAGE);
		return -1;
	}
	if (myself.w_cstst == LOGIN_CSTAT)
	{
		memset(myself.w_buf, 0, MAX_MSG_SIZE);
		enc_logout_msg(myself.w_buf, &n, 0);
		send(myself.w_sockfd, myself.w_buf, n, 0);
		myself.w_mstat = MSG_RCVING;
	}

	return -2;
}

int help_cmd_proc(char *n_tok)
{
	char *p[HELP_CMD_FLDS];

	if (cmdline_proc(n_tok, p, HELP_CMD_FLDS, 1) == -1)
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, HELP_C_USAGE);
		return -1;
	}

	fprintf(stdout, "%s", CLIENT_USAGE);
	return 0;
}

int debug_cmd_proc(char *n_tok)
{
	CLIENT_FRND *pos;
	printf("client: id=%d, name=%s, password=%s\n", myself.w_id, myself.w_name, myself.w_pass);
	printf("client: sckfd=%d, cstat=%d, friend cnt=%d\n", myself.w_sockfd, myself.w_cstst, myself.w_fndcnt);
	

	ufrnd_for_each_entry(pos, &myself.w_flisthd, c_node)
	{
		printf("client: friend id=%d, name=%s, stat=%d", pos->c_id, pos->c_name, pos->c_stat);
	}

	return 0;

}

int usr_stat_check(char *token)
{
	//printf("usr_stat_check, token=\"%s\"\n", token);
	if (_stricmp(token, REG_CMD) &&
		_stricmp(token, LIN_CMD) &&
		_stricmp(token, HELP_CMD) &&
		_stricmp(token, MESG_CMD) &&
		_stricmp(token, EXIT_CMD) )
	{
		if (myself.w_cstst != LOGIN_CSTAT)
		{
			fprintf(stderr, "%s", INV_STST_USAGE);
			return -1;
		}
	}
	return 0;

	
}
/*
命令处理函数
@buf: 用户输入的命令字符串缓冲区
*/
int usr_cmd_process(char *buf)
{
	char *token = NULL, *next_token = NULL, seps[] = " ";
	token = strtok_s(buf, seps, &next_token);
	//printf("token=%s\n", token);

	if (!token) return 0;

	if (usr_stat_check(token) < 0) return -1;
	
	if (!_stricmp(token, REG_CMD))
	{
		//printf("entry reg cmd proc!, next_token=\"%s\"\n",next_token);
		return reg_cmd_proc(next_token);
	}
	else if (!_stricmp(token, LIN_CMD))
	{
		return login_cmd_proc(next_token);
	}
	else if (!_stricmp(token, FLST_CMD))
	{
		return flist_cmd_proc(next_token);
	}
	else if (!_stricmp(token, FADD_CMD))
	{
		return add_cmd_proc(next_token);
	}
	else if (!_stricmp(token, FDEL_CMD))
	{
		return del_cmd_proc(next_token);
	}
	else if (!_stricmp(token, ALIST_CMD))
	{
		return alist_cmd_proc(next_token);
	}
	else if (!_stricmp(token, FSTAT_CMD))
	{
		return fstat_cmd_proc(next_token);
	}
	else if (!_stricmp(token, CHAT_CMD))
	{
		return chat_cmd_proc(next_token);
	}
	else if (!_stricmp(token, EXIT_CMD))
	{
		return logout_cmd_proc(next_token);
	}
	else if (!_stricmp(token, MESG_CMD))
	{
		return mesg_cmd_proc(next_token);
	}
	else if (!_stricmp(token, DEBUG_CMD))
	{
		return debug_cmd_proc(next_token);
	}
	else if (!_stricmp(token, HELP_CMD))
	{
		return help_cmd_proc(next_token);
	}
	else 
	{
		fprintf(stderr, "%s%s", INV_C_USAGE, CLIENT_USAGE);
		return -1;
	}
}




