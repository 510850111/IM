#pragma once
#ifndef CLIENT_LOG_H
#define CLIENT_LOG_H
/*
������Ϣ�Ľṹ

������		ID		10001
������		ID		10002
����ʱ��		ʱ���ַ���
��Ϣ������	�ַ���

�ı��ļ�

������ / ������ / ����ʱ�� / ������Ϣ


����

�洢   ����   ����

* /


/*��Ϣ�ָ���*/
#define FILED_SEPS "/"
/*ÿһ�е��ֶθ���*/
#define MSG_FIELDS 4

#define CLIENT_LOG_FILE "msg_bakup.txt"


void client_log_save(char *msg, int from, int to);
void client_log_export(FILE *sfp, FILE *dfp, int from, int to);


#endif // !CLIENT_LOG_H	
