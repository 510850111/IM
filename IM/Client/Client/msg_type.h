#pragma once

/*实现通用消息结构*/
#define SERVER_FORWARD_FLAG (0x1 << 15)


typedef struct msg_header
{
	/*消息类型*/
	unsigned short msg_type;
	/*消息长度*/
	unsigned short msg_len;
	/*可变长度的消息内容,虚拟的消息内容指针，不占空间，仅仅是一个占位符*/
	char msg_data[0];
}MSG_HDR;

/*所有的消息类型 msg_type*/
/*用户注册消息*/
#define MSG_REG		0x01
/*用户的登录消息*/
#define MSG_LOGIN	0x02
/*好友管理信息*/
#define MSG_FRNDMGT	0x03
/*用户聊天消息*/
#define MSG_CHAT	0x04
/*退出登录消息*/
#define MSG_LOGOUT	0x05

/*好友管理消息子类型*/
#define F_LREG		0x01
#define F_ADD		0x02
#define	F_DEL		0x03
#define F_ALST		0x04
#define F_STAT		0x05


/*最大的用户名长度*/
#define MAXNAME_LEN			16	
/*最大的用户密码长度*/
#define MAX_USERPASS_LEN	6
/*注册消息与注册回复消息*/
typedef	struct reg_msgdata
{
	char r_name[MAXNAME_LEN];
	char r_passwd[MAX_USERPASS_LEN];
}REG_MSG;

#define MAX_ERR_LEN		80
typedef struct reg_msg_resp
{
	/*将uid返回给客户端，-1表示出错*/
	int re_id;
	/*出错的原因*/
	char re_reason[0];
}REG_RESP;

/*登录消息与登录回复消息*/
typedef struct login_msg
{
	int lg_id;
	char lg_pass[MAX_USERPASS_LEN];
}LIG_MSG;

#define LOGIN_OK	1
#define LOGIN_ERR	2

typedef struct login_resp
{
	/*登录状态 1表示ok  2表示出错*/
	int lg_stat;
	char lg_name[MAXNAME_LEN];
	char lg_reason[0];
}LIG_RESP;

/*好友操作结构体与回复*/
typedef struct frnd_op
{
	/*用户本身*/
	int f_id;
	/*表示需要操作的用户id数组*/
	int f_fids[0];
}FRND_OP;

/*好友的在线状态*/
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
	/*成功数目*/
	short fre_num;
	/*好友的状态*/
	FRND_ST fre_ok_frnd[0];
}FRND_RESP;

/*聊天信息*/
typedef struct chat_msg
{
	/*自身id*/
	int ch_sid;
	/*接收者id*/
	int ch_rid;
	/*需要发送的文本信息*/
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


