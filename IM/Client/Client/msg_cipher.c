#include "msg_cipher.h"

void msg_encipher(char *msg, int key)
{
	/*遍历数组，进行加密*/
	for (int i = 0; msg[i]; i++)
	{
		/*大小写字母+key*/
		/*如果超出范围，则做回绕*/
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
	/*解密*/
	/*大小写字母-key*/
	/*如果超出范围，则做回绕*/
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


