#ifndef USR_MGT_H

#define USR_MGT_H
#include<stdio.h>
#include<string.h>


/*系统支持的用户最大数目*/
#define MAX_UNUM	50
/*系统支持的最大用户姓名长度*/
#define MAX_NLEN	32

#define USER_OFFLINE	0
#define USER_ONLINE		1

/*表示用户的结构体*/
struct susr
{
	/*用户昵称*/
	char u_name[MAX_NLEN];
	/*用户ID，唯一*/
	int	u_id;
	/*用户在线状态*/
	int u_stat;
};

void usr_ary_init(int id, char *name);

/*增加好友,0表示成功，-1表示失败*/
int usr_add(int uid, char *name);
/*删除好友，0表示成功，-1表示失败*/
int usr_del(int uid);

/*好友的排序*/
void usr_sort_by_name(struct susr *p);
void usr_sort_by_id(struct susr *p);

/*通过好友昵称或者id查找好友*/
struct susr *usr_find_by_name(char *name);
struct susr *usr_find_by_id(int id);

/*使用二分法查找好友*/
struct susr *binary_search_id(int id);

/*导出好友信息*/
void usr_info_dump();

/*测试*/
void usr_add_test();
void usr_del_test();
void usr_mgt_test_all(void);

#endif // !USR_MGT_H