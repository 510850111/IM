#pragma once
#ifndef CUSR_CMD_H
#define CUSR_CMD_H

#define	C_LOC	1
#define	C_SERV	2
typedef struct usr_md
{
	char *u_str;
	short u_local;
	short u_mtype;
	int  u_optcnt;
}USR_CMD;


#define MAX_LINE_LEN	256
#define MAX_PROMPT_LEN	64

#define		USER_PROM	"im_client"
#define		REG_CMD		"reg"
#define		LIN_CMD		"login"
#define		FLST_CMD	"flist"
#define		FADD_CMD	"add"
#define		FDEL_CMD	"del"
#define		ALIST_CMD	"alist"
#define		FSTAT_CMD	"stat"
#define		CHAT_CMD	"talk"
#define		EXIT_CMD	"exit"
#define		HELP_CMD	"help"
#define		MESG_CMD	"mesg"
#define		DEBUG_CMD	"debug"

#define		REG_CMD_FLDS	4
#define		LOGIN_CMD_FLDS	3
#define		FLIST_CMD_FLDS	1
#define		ADD_CMD_FLDS	2
#define		DEL_CMD_FLDS	2
#define		ALIST_CMD_FLDS	1
#define		FSTST_CMD_FLDS	2
#define		LOGOUT_CMD_FLDS 1
#define		ALIST_CMD_FLDS	1
#define		CHAT_CMD_FLDS	2
#define		HELP_CMD_FLDS	1
#define		DEBUG_CMD_FLDS	1
#define		MESG_CMD_FLDS	3


#define		REG_C_USAGE		"reg <name> <passwd> <passwd> ͨ���û���������ע���û���\n"
#define		LIN_C_USAGE		"login <uid> <password>  �û���¼��\n"
#define		FLIST_C_USAGE	"flist �г����еĺ����û���\n"
#define		FADD_C_USAGE	"add <uid> ���ݺ���id��Ӻ��ѡ�\n"
#define		FDEL_C_USAGE	"del <uid> ���ݺ���idɾ�����ѡ�\n"
#define		ALIST_C_USAGE	"alist ��ѯ���е������û���\n"
#define		FSTAT_C_USAGE	"stat <uid> ��ѯ����״̬��\n"
#define		CHAT_C_USAGE	"talk <uid> <msg> ������Ϣ�����ѡ�\n"
#define		EXIT_C_USAGE	"exit �û��˳���\n"
#define		HELP_C_USAGE	"help ��ӡ������Ϣ��\n"
#define		MESG_C_USAGE	"mesg <from> <to> ��ʾ��form�û���to�û��������¼��\n"
#define		DEBUG_C_USAGE	"debug ��ʾ��ǰ�û���Ϣ��\n"

#define		CLIENT_C_USAGE	"�ͻ�����������������(<>Ϊ��ѡ����)��\n"
#define		CLIENT_USAGE		\
			CLIENT_C_USAGE		\
			REG_C_USAGE			\
			LIN_C_USAGE			\
			FLIST_C_USAGE		\
			FADD_C_USAGE		\
			FDEL_C_USAGE		\
			ALIST_C_USAGE		\
			FSTAT_C_USAGE		\
			CHAT_C_USAGE		\
			EXIT_C_USAGE		\
			HELP_C_USAGE		\
			MESG_C_USAGE		\
			DEBUG_C_USAGE		\



#define		INV_C_USAGE		"�޷�����������û�����\n\n"
#define		INV_STST_USAGE	"���ȵ�¼��\n\n"

extern USR_CMD all_cmnds[];


int cmdline_proc(char *n_tok, char **p, int cnt, int strict);
int client_send(char *buf, int n);
int reg_cmd_proc(char *n_tok);
int login_cmd_proc(char *n_tok);
int mesg_cmd_proc(char *n_tok);
int flist_cmd_proc(char *n_tok);
int add_cmd_proc(char *n_tok);
int del_cmd_proc(char *n_tok);
int alist_cmd_proc(char *n_tok);
int fstat_cmd_proc(char *n_tok);
int chat_cmd_proc(char *n_tok);
int logout_cmd_proc(char *n_tok);
int help_cmd_proc(char *n_tok);
int usr_cmd_process(char *buf);
int debug_cmd_proc(char *n_tok);
int usr_stat_check(char *token);

#endif // !CUSR_CMD_H
