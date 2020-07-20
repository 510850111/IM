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


	/*提示用户输入用户名密码*/
	printf("请输入用户名：");
	scanf("%s", name);
	printf("\n");
	printf("请输入密码：");
	scanf("%s", passwd);

	/*判断输入的用户名是否合法*/
	if (strcmp(name, USERNAME) != 0)
	{
		printf("用户名与密码不匹配！\n");
		return -1;
	}

	/*判断输入的密码是否正确*/
	if (strcmp(passwd, PASSWD) != 0)
	{
		printf("用户名与密码不匹配！\n");
		return -1;
	}

	printf("用户名密码匹配！");
}


int check_for_users(int user, char *passwd)
{

	int result = -1;	//0表示成功，其他表示失败

	printf("请输入用户名：");
	scanf("%d", &user);
	printf("\n");
	printf("请输入密码：");
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
		printf("用户名密码匹配！\n");
	}
	else
	{
		printf("用户名密码不匹配！\n");
	}

	return result;
}