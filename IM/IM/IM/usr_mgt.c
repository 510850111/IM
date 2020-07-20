#include "usr_mgt.h"

/*所有的好友数组，最多支持 MAX_ULEN个*/
static struct susr usr_ary[MAX_UNUM];

/*实际的总的好友数目*/
static int usr_cnt = 0;

/*代表是通信用户本身的结构体*/
static	struct susr usr_self;

/*好友数组初始化*/
void usr_ary_init(int id, char *name)
{
	memset(usr_ary, 0, sizeof(usr_ary));

	usr_cnt = 0;
	memset(&usr_self, 0, sizeof(usr_self));

	usr_self.u_id = id;
	strcpy(usr_self.u_name, name);
	usr_self.u_stat = USER_OFFLINE;
}

/*好友信息输出*/
void usr_info_dump()
{
	struct susr *p = usr_ary, *pend = p + usr_cnt;
	printf("所有好友信息如下：\n user:%d \t name:%s \t user cnt: %d \n", usr_self.u_id, usr_self.u_name, usr_cnt);

	int i = 0;
	while (p < pend)
	{
		i++;
		printf(" the %dth user  \t name:%s \t id:%d\n", i, p->u_name, p->u_id);
		p++;

	}
	printf("\n");

}

/*查找好友*/
/*通过昵称查找好友*/
struct susr *usr_find_by_name(char *name)
{
	struct susr *p = usr_ary, *pend = p + usr_cnt;
	while (p < pend)
	{
		if (!strcmp(p->u_name, name))	break;
		p++;
	}

	return (p == pend) ? NULL : p;
}


struct susr *usr_find_by_id(int id)
{
	struct susr *p = usr_ary, *pend = p + usr_cnt;

	/*判断uid是否为0，或者为用户自身id，返回空*/
	if (!id || id == usr_self.u_id)	return NULL;

	/*遍历好友列表*/
	while (p < pend)
	{
		if (p->u_id == id) break;
		p++;
	}

	return (p == pend) ? NULL : p;
}

void usr_sort_by_name(struct susr *p)
{
	/*选择排序*/
	int i, j, index;
	struct susr temp;
	for ( i = 0; i < usr_cnt -1; i++)
	{
		index = i;
		for ( j = i+1; j < usr_cnt; j++)
		{
			if (strcmp(usr_ary[j].u_name, usr_ary[index].u_name) < 0)
			{
				index = j;
			}
		}
		if (index != i)
		{
			temp = usr_ary[index];
			usr_ary[index] = usr_ary[i];
			usr_ary[i] = temp;
		}
	}

}

void usr_sort_by_id(struct susr *p)
{
	/*选择排序*/
	int i, j, index;
	struct susr temp;
	for (i = 0; i < usr_cnt - 1; i++)
	{
		index = i;
		for (j = i + 1; j < usr_cnt; j++)
		{
			if (usr_ary[j].u_id < usr_ary[index].u_id )
			{
				index = j;
			}
		}
		if (index != i)
		{
			temp = usr_ary[index];
			usr_ary[index] = usr_ary[i];
			usr_ary[i] = temp;
		}
	}
}

/*增加好友,0表示成功，-1表示失败*/
int usr_add(int uid, char *name)
{
	/*判断 好友id不能为0  好友id不能为自身id  好友数目不能达到上限MAX_UNUM*/
	/*判断 好友是否已经是自己的好友了 usr_find_by_id*/
	struct susr *p, *usr;
	if (!uid || uid == usr_self.u_id || usr_cnt == MAX_UNUM)
	{
		return -1;
	}

	if ((usr = usr_find_by_id(uid)) != NULL)
	{
		return -1;
	}

	/*将好友插入到已有的好友的末尾*/
	p = &usr_ary[usr_cnt];
	p->u_id = uid;
	strcpy(p->u_name, name);
	p->u_stat = USER_OFFLINE;

	/*更新好友数目*/
	usr_cnt++;
	
	return 0;
}

/*删除好友，0表示成功，-1表示失败*/
int usr_del(int uid)
{
	struct susr *usr, *p, *pend = usr_ary + usr_cnt;

	/*查找好友是否在好友列表中*/
	if ((usr = usr_find_by_id(uid)) == NULL)
	{
		return -1;
	}

	/*已经找到该好友的对应的结构*/
	p = usr + 1;
	/*依次对被删除好友之后的结构往前移动，直到最后一个好友结构*/
	while (p <pend)
	{
		*usr = *p++;
	}
	memset(p - 1, 0, sizeof(*p));

	/*更新好友计数*/
	usr_cnt--;

	return 0;

}


/*测试*/
void usr_add_test()
{
	usr_add(10000, "tom");
	usr_add(10001, "mary");
	usr_add(10002, "jack");
	usr_add(10003, "jeo");
	usr_add(10004, "cat");
	usr_add(10005, "apple");
	usr_add(10006, "banana");
	usr_add(10007, "orange");
	usr_add(10008, "lisa");
	usr_add(10009, "alice");
}

void usr_del_test()
{
	usr_del(10001);
	usr_del(20000);
	usr_del(10008);
	usr_del(10010);
	usr_del(999999);
}

void usr_mgt_test_all(void)
{
	struct susr *usr;

	/*初始化好友数组*/
	usr_ary_init(1, "brant");

	/*好友增加测试*/
	printf("\n====================好友增加测试=======================\n");
	usr_add_test();
	usr_info_dump();

	printf("\n====================好友查找测试=======================\n");
	usr = usr_find_by_id(10009);
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("10009好友未找到\n");

	usr = usr_find_by_id(15000);
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("15000好友未找到\n");

	usr = usr_find_by_name("banana");
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("banana 好友未找到\n");

	usr = usr_find_by_name("ASDASF");
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("ASDASF 好友未找到\n");

	/*通过昵称排序测试*/
	printf("\n====================好友昵称排序=======================\n");
	usr_sort_by_name(NULL);
	usr_info_dump();

	printf("\n====================好友id排序=======================\n");
	usr_sort_by_id(NULL);
	usr_info_dump();

	/*删除好友测试*/
	printf("\n====================好友删除测试=======================\n");
	usr_del_test();
	usr_info_dump();

}



