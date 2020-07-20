#ifndef CHAT_LOG_H
#define CHAT_LOG_H

/*
聊天信息的结构

发送者		ID		10001
接收者		ID		10002
发送时间		时间字符串
消息的内容	字符串

文本文件

发送者/接收者/发送时间/具体消息


功能

存储   导入   导出

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*所支持的最大消息长度*/
#define MAXLINE	512
/*消息分隔符*/
#define FILED_SEPS "/"
/*每一行的字段个数*/
#define MSG_FIELDS 4

void backup_single_msg(char *msg, int from, int to, FILE *fp);
void export_msg_from_to_sb(FILE *sfp, FILE *dfp, int from, int to);

#endif // !CHAT_LOG_H

