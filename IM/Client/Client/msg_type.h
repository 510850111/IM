#pragma once

/*ʵ��ͨ����Ϣ�ṹ*/
#define SERVER_FORWARD_FLAG (0x1 << 15)


typedef struct msg_header
{
	/*��Ϣ����*/
	unsigned short msg_type;
	/*��Ϣ����*/
	unsigned short msg_len;
	/*�ɱ䳤�ȵ���Ϣ����,�������Ϣ����ָ�룬��ռ�ռ䣬������һ��ռλ��*/
	char msg_data[0];
}MSG_HDR;

/*���е���Ϣ���� msg_type*/
/*�û�ע����Ϣ*/
#define MSG_REG		0x01
/*�û��ĵ�¼��Ϣ*/
#define MSG_LOGIN	0x02
/*���ѹ�����Ϣ*/
#define MSG_FRNDMGT	0x03
/*�û�������Ϣ*/
#define MSG_CHAT	0x04
/*�˳���¼��Ϣ*/
#define MSG_LOGOUT	0x05

/*���ѹ�����Ϣ������*/
#define F_LREG		0x01
#define F_ADD		0x02
#define	F_DEL		0x03
#define F_ALST		0x04
#define F_STAT		0x05


/*�����û�������*/
#define MAXNAME_LEN			16	
/*�����û����볤��*/
#define MAX_USERPASS_LEN	6
/*ע����Ϣ��ע��ظ���Ϣ*/
typedef	struct reg_msgdata
{
	char r_name[MAXNAME_LEN];
	char r_passwd[MAX_USERPASS_LEN];
}REG_MSG;

#define MAX_ERR_LEN		80
typedef struct reg_msg_resp
{
	/*��uid���ظ��ͻ��ˣ�-1��ʾ����*/
	int re_id;
	/*�����ԭ��*/
	char re_reason[0];
}REG_RESP;

/*��¼��Ϣ���¼�ظ���Ϣ*/
typedef struct login_msg
{
	int lg_id;
	char lg_pass[MAX_USERPASS_LEN];
}LIG_MSG;

#define LOGIN_OK	1
#define LOGIN_ERR	2

typedef struct login_resp
{
	/*��¼״̬ 1��ʾok  2��ʾ����*/
	int lg_stat;
	char lg_name[MAXNAME_LEN];
	char lg_reason[0];
}LIG_RESP;

/*���Ѳ����ṹ����ظ�*/
typedef struct frnd_op
{
	/*�û�����*/
	int f_id;
	/*��ʾ��Ҫ�������û�id����*/
	int f_fids[0];
}FRND_OP;

/*���ѵ�����״̬*/
#define USR_ONLINE		1
#define USR_OFFLINE		2

typedef struct frnd_stat
{
	char fs_name[MAXNAME_LEN];
	int fs_id;
	int fs_stat;
}FRND_ST;

#define OP_ALL_OK	1
#define OP_PART_OD	2
#define OP_ALL_FAIL	3

typedef struct frnd_op_resp
{
	short fre_stat;
	/*�ɹ���Ŀ*/
	short fre_num;
	/*���ѵ�״̬*/
	FRND_ST fre_ok_frnd[0];
}FRND_RESP;

/*������Ϣ*/
typedef struct chat_msg
{
	/*����id*/
	int ch_sid;
	/*������id*/
	int ch_rid;
	/*��Ҫ���͵��ı���Ϣ*/
	char ch_msg[0];
}CHAT_MSG;

#define SND_OK		1
#define SND_ERR		2
typedef struct chat_resp
{
	int c_stat;
	char c_reason[0];
}CHAT_RESP;

typedef struct logout_msg
{
	int lg_id;
	char lg_pass[MAX_USERPASS_LEN];
}LOUT_MSG;


