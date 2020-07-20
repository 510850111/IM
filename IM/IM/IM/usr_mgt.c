#include "usr_mgt.h"

/*���еĺ������飬���֧�� MAX_ULEN��*/
static struct susr usr_ary[MAX_UNUM];

/*ʵ�ʵ��ܵĺ�����Ŀ*/
static int usr_cnt = 0;

/*������ͨ���û�����Ľṹ��*/
static	struct susr usr_self;

/*���������ʼ��*/
void usr_ary_init(int id, char *name)
{
	memset(usr_ary, 0, sizeof(usr_ary));

	usr_cnt = 0;
	memset(&usr_self, 0, sizeof(usr_self));

	usr_self.u_id = id;
	strcpy(usr_self.u_name, name);
	usr_self.u_stat = USER_OFFLINE;
}

/*������Ϣ���*/
void usr_info_dump()
{
	struct susr *p = usr_ary, *pend = p + usr_cnt;
	printf("���к�����Ϣ���£�\n user:%d \t name:%s \t user cnt: %d \n", usr_self.u_id, usr_self.u_name, usr_cnt);

	int i = 0;
	while (p < pend)
	{
		i++;
		printf(" the %dth user  \t name:%s \t id:%d\n", i, p->u_name, p->u_id);
		p++;

	}
	printf("\n");

}

/*���Һ���*/
/*ͨ���ǳƲ��Һ���*/
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

	/*�ж�uid�Ƿ�Ϊ0������Ϊ�û�����id�����ؿ�*/
	if (!id || id == usr_self.u_id)	return NULL;

	/*���������б�*/
	while (p < pend)
	{
		if (p->u_id == id) break;
		p++;
	}

	return (p == pend) ? NULL : p;
}

void usr_sort_by_name(struct susr *p)
{
	/*ѡ������*/
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
	/*ѡ������*/
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

/*���Ӻ���,0��ʾ�ɹ���-1��ʾʧ��*/
int usr_add(int uid, char *name)
{
	/*�ж� ����id����Ϊ0  ����id����Ϊ����id  ������Ŀ���ܴﵽ����MAX_UNUM*/
	/*�ж� �����Ƿ��Ѿ����Լ��ĺ����� usr_find_by_id*/
	struct susr *p, *usr;
	if (!uid || uid == usr_self.u_id || usr_cnt == MAX_UNUM)
	{
		return -1;
	}

	if ((usr = usr_find_by_id(uid)) != NULL)
	{
		return -1;
	}

	/*�����Ѳ��뵽���еĺ��ѵ�ĩβ*/
	p = &usr_ary[usr_cnt];
	p->u_id = uid;
	strcpy(p->u_name, name);
	p->u_stat = USER_OFFLINE;

	/*���º�����Ŀ*/
	usr_cnt++;
	
	return 0;
}

/*ɾ�����ѣ�0��ʾ�ɹ���-1��ʾʧ��*/
int usr_del(int uid)
{
	struct susr *usr, *p, *pend = usr_ary + usr_cnt;

	/*���Һ����Ƿ��ں����б���*/
	if ((usr = usr_find_by_id(uid)) == NULL)
	{
		return -1;
	}

	/*�Ѿ��ҵ��ú��ѵĶ�Ӧ�Ľṹ*/
	p = usr + 1;
	/*���ζԱ�ɾ������֮��Ľṹ��ǰ�ƶ���ֱ�����һ�����ѽṹ*/
	while (p <pend)
	{
		*usr = *p++;
	}
	memset(p - 1, 0, sizeof(*p));

	/*���º��Ѽ���*/
	usr_cnt--;

	return 0;

}


/*����*/
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

	/*��ʼ����������*/
	usr_ary_init(1, "brant");

	/*�������Ӳ���*/
	printf("\n====================�������Ӳ���=======================\n");
	usr_add_test();
	usr_info_dump();

	printf("\n====================���Ѳ��Ҳ���=======================\n");
	usr = usr_find_by_id(10009);
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("10009����δ�ҵ�\n");

	usr = usr_find_by_id(15000);
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("15000����δ�ҵ�\n");

	usr = usr_find_by_name("banana");
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("banana ����δ�ҵ�\n");

	usr = usr_find_by_name("ASDASF");
	if (usr)	printf("%d \t %s \t \n", usr->u_id, usr->u_name);
	else printf("ASDASF ����δ�ҵ�\n");

	/*ͨ���ǳ��������*/
	printf("\n====================�����ǳ�����=======================\n");
	usr_sort_by_name(NULL);
	usr_info_dump();

	printf("\n====================����id����=======================\n");
	usr_sort_by_id(NULL);
	usr_info_dump();

	/*ɾ�����Ѳ���*/
	printf("\n====================����ɾ������=======================\n");
	usr_del_test();
	usr_info_dump();

}



