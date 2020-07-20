#include "im_client.h"

void client_log_save(char *msg, int from, int to)
{
	FILE *fp;
	char buf[MAX_LINE_LEN];
	time_t ticks;
	ticks = time(NULL);
	int n = 0;

	fp = fopen(CLIENT_LOG_FILE, "a+");
	if (fp == NULL)
	{
		perror("client-log: open for append error!\n");
		return;
	}

	n += _snprintf(buf + n, sizeof(buf) - n, "%d/", from);
	n += _snprintf(buf + n, sizeof(buf) - n, "%d/", to);
	n += _snprintf(buf + n, sizeof(buf) - n, "%.24s/", ctime(&ticks));
	n += _snprintf(buf + n, sizeof(buf) - n, "%s\n", msg);

	buf[n] = '\0';

	fputs(buf, fp);

	fclose(fp);
}


void client_log_export(FILE *sfp, FILE *dfp, int from, int to)
{
	char buf[MAX_LINE_LEN];
	char *token = NULL, *next_token = NULL, seps[] = "/";
	char *p[MSG_FIELDS] = { NULL };

	int msg_src, msg_dst, i;

	memset(buf, 0, sizeof(buf));
	while (fgets(buf, MAX_LINE_LEN, sfp) != NULL)
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
			|| (from && !to && msg_src == from)
			|| (!from && to && msg_dst == to)
			|| (from && to && msg_src == from && msg_dst == to))
		{
			fprintf(dfp, "client-log: %s->%s, Time:%s, msg:%s\n", p[0], p[1], p[2], p[3]);
		}

		memset(buf, 0, sizeof(buf));

	}
}