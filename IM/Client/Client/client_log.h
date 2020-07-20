#pragma once
#ifndef CLIENT_LOG_H
#define CLIENT_LOG_H
/*
聊天信息的结构

发送者		ID		10001
接收者		ID		10002
发送时间		时间字符串
消息的内容	字符串

文本文件

发送者 / 接收者 / 发送时间 / 具体消息


功能

存储   导入   导出

* /


/*消息分隔符*/
#define FILED_SEPS "/"
/*每一行的字段个数*/
#define MSG_FIELDS 4

#define CLIENT_LOG_FILE "msg_bakup.txt"


void client_log_save(char *msg, int from, int to);
void client_log_export(FILE *sfp, FILE *dfp, int from, int to);


#endif // !CLIENT_LOG_H	
