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
		from �� to��Ϊ0��Ҫ����������Ϣ
		from��Ϊ0�� toΪ0�� ����msg_src==from����������ĳ���û����͵���Ϣ
		fromΪ0�� to��Ϊ0�� msg_dst==to�� ���˷�����ĳ���û��ĵ���Ϣ
		from��to����Ϊ0�� ���� msg_src==from && msg_dst==to�����˷�������from���ҽ�������to����Ϣ
		
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