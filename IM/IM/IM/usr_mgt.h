#ifndef USR_MGT_H

#define USR_MGT_H
#include<stdio.h>
#include<string.h>


/*ϵͳ֧�ֵ��û������Ŀ*/
#define MAX_UNUM	50
/*ϵͳ֧�ֵ�����û���������*/
#define MAX_NLEN	32

#define USER_OFFLINE	0
#define USER_ONLINE		1

/*��ʾ�û��Ľṹ��*/
struct susr
{
	/*�û��ǳ�*/
	char u_name[MAX_NLEN];
	/*�û�ID��Ψһ*/
	int	u_id;
	/*�û�����״̬*/
	int u_stat;
};

void usr_ary_init(int id, char *name);

/*���Ӻ���,0��ʾ�ɹ���-1��ʾʧ��*/
int usr_add(int uid, char *name);
/*ɾ�����ѣ�0��ʾ�ɹ���-1��ʾʧ��*/
int usr_del(int uid);

/*���ѵ�����*/
void usr_sort_by_name(struct susr *p);
void usr_sort_by_id(struct susr *p);

/*ͨ�������ǳƻ���id���Һ���*/
struct susr *usr_find_by_name(char *name);
struct susr *usr_find_by_id(int id);

/*ʹ�ö��ַ����Һ���*/
struct susr *binary_search_id(int id);

/*����������Ϣ*/
void usr_info_dump();

/*����*/
void usr_add_test();
void usr_del_test();
void usr_mgt_test_all(void);

#endif // !USR_MGT_H