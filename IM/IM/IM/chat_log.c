#include "chat_log.h"

void backup_single_msg(char *msg, int from, int to, FILE *fp)
{
	char buf[MAXLINE];
	time_t ticks;
	ticks = time(NULL);
	int n=0;

	n += _snprintf(buf + n, sizeof(buf) - n, "%d/", from);
	n += _snprintf(buf + n, sizeof(buf) - n, "%d/", to);
	n += _snprintf(buf + n, sizeof(buf) - n, "%.24s/", ctime(&ticks));
	n += _snprintf(buf + n, sizeof(buf) - n, "%s\n", msg);

	buf[n] = '\0';

	fputs(buf, fp);
}


void export_msg_from_to_sb(FILE *sfp, FILE *dfp, int from, int to)
{
	char buf[MAXLINE];
	char *token = NULL, *next_token = NULL, seps[] = "/";
	char p[MSG_FIELDS] = { NULL };
	
	int msg_src, msg_dst, i;
	
	memset(buf, 0, sizeof(buf));
	while (fgets(buf, MAXLINE, sfp) != NULL)
	{
		token = strtok_s(buf, seps, &next_token);
		for (i = 0; i < MSG_FIELDS; i++)
		{
			p[i] = token;
			token = strtok_s(NULL, seps, &next_token);
		}
		if (from)
		{
			msg_src = atoi(p[0]);
		}
		if (to)
		{
			msg_dst = atoi(p[1]);
		}
		/*
		from 和 to均为0，要导出所有消息
		from不为0， to为0， 并且msg_src==from，过滤来自某个用户发送的消息
		from为0， to不为0， msg_dst==to。 过滤发送至某个用户的的消息
		from与to都不为0， 并且 msg_src==from && msg_dst==to，过滤发送者是from并且接收者是to的消息
		
		*/
		if ((!from && !to) 
			|| (from && !to && msg_src==from)
			|| (!from && to && msg_dst==to)
			|| (from && to && msg_src==from && msg_dst==to))
		{
			fprintf(dfp, "From:%s, To:%s, Time:%s, msg:%s\n",p[0], p[1], p[2], p[3]);
		}

		memset(buf, 0, sizeof(buf));

	}
}