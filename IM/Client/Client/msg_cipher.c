#include "msg_cipher.h"

void msg_encipher(char *msg, int key)
{
	/*�������飬���м���*/
	for (int i = 0; msg[i]; i++)
	{
		/*��Сд��ĸ+key*/
		/*���������Χ����������*/
		if (msg[i] >= 'a' && msg[i] <= 'z')
		{
			msg[i] = (msg[i] + key > 'z') ? (msg[i] + key - ('z' - 'a' + 1)) : (msg[i] + key);

		}
		else if (msg[i] >= 'A' && msg[i] <= 'Z')
		{
			msg[i] = (msg[i] + key > 'Z') ? (msg[i] + key - 26) : (msg[i] + key);
		}
		else
		{
			continue;
		}
	}
}
void msg_decipher(char *msg, int key)
{
	/*����*/
	/*��Сд��ĸ-key*/
	/*���������Χ����������*/
	for (int i = 0; msg[i]; i++)
	{
		if (msg[i] >= 'a' && msg[i] <= 'z')
		{
			msg[i] = (msg[i] - key < 'a') ? (msg[i] - key + ('z' - 'a' + 1)) : (msg[i] - key);
		}
		else if (msg[i] >= 'A' && msg[i] <= 'Z')
		{
			msg[i] = (msg[i] - key < 'A') ? (msg[i] - key + ('Z' - 'A' + 1)) : (msg[i] - key);
		}
		else
		{
			continue;
		}

	}
}


