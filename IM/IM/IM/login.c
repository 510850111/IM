#include<stdio.h>

#define USERNAME	"USER1"
#define PASSWD		"PASSWD1"


#define USER1 10001
#define PASSWD1 "APPLE"
#define USER2 10002
#define PASSWD2 "BANANA"
#define USER3 10003
#define PASSWD3 "CAT"

int user;
char passwd[40];

int checkForSingle(char *name, char *passwd)
{


	/*��ʾ�û������û�������*/
	printf("�������û�����");
	scanf("%s", name);
	printf("\n");
	printf("���������룺");
	scanf("%s", passwd);

	/*�ж�������û����Ƿ�Ϸ�*/
	if (strcmp(name, USERNAME) != 0)
	{
		printf("�û��������벻ƥ�䣡\n");
		return -1;
	}

	/*�ж�����������Ƿ���ȷ*/
	if (strcmp(passwd, PASSWD) != 0)
	{
		printf("�û��������벻ƥ�䣡\n");
		return -1;
	}

	printf("�û�������ƥ�䣡");
}


int check_for_users(int user, char *passwd)
{

	int result = -1;	//0��ʾ�ɹ���������ʾʧ��

	printf("�������û�����");
	scanf("%d", &user);
	printf("\n");
	printf("���������룺");
	scanf("%s", passwd);

	switch (user)
	{
	case USER1:
		result = strcmp(passwd, PASSWD1);
		break;
	case USER2:
		result = strcmp(passwd, PASSWD2);
		break;
	case USER3:
		result = strcmp(passwd, PASSWD3);
		break;
	default:
		result = -1;
		break;
	}

	if (result == 0)
	{
		printf("�û�������ƥ�䣡\n");
	}
	else
	{
		printf("�û������벻ƥ�䣡\n");
	}

	return result;
}