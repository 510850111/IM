#ifndef CHAT_LOG_H
#define CHAT_LOG_H

/*
������Ϣ�Ľṹ

������		ID		10001
������		ID		10002
����ʱ��		ʱ���ַ���
��Ϣ������	�ַ���

�ı��ļ�

������/������/����ʱ��/������Ϣ


����

�洢   ����   ����

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*��֧�ֵ������Ϣ����*/
#define MAXLINE	512
/*��Ϣ�ָ���*/
#define FILED_SEPS "/"
/*ÿһ�е��ֶθ���*/
#define MSG_FIELDS 4

void backup_single_msg(char *msg, int from, int to, FILE *fp);
void export_msg_from_to_sb(FILE *sfp, FILE *dfp, int from, int to);

#endif // !CHAT_LOG_H

