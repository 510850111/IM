#include "chat_log.h"

int main(int argc, char **argv)
{
	FILE *fp;
	char * msg_sample[] = {
		"Great minds have purpose, others have wishes.",
		"Being single is better than being in an unfaithful relationship.",
		"If you find a path with no obstacles, it probably doesn¡¯t lead anywhere.",
		"Getting out of bed in winter is one of life¡¯s hardest mission.",
		"The future is scary but you can¡¯t just run to the past cause it¡¯s familiar.",
		"I love it when I catch you looking at me then you smile and look away.",
		"Having a calm smile to face with being disdained indicates kind of confidence.",
		"Success is the ability to go from one failure to another with no loss of enthusiasm.",
		"Not everything that is faced can be changed, but nothing can be changed until it is faced.",
		"A guy who whispers in your ears, saying ¡± It¡¯s alright, I¡¯m here.¡± Holds you when you¡¯re sad, and treasures everything about you. That¡¯s the guy I want to give my heart to."

	};
	if (argc != 2)
	{
		printf("usage %s <save file>\n",argv[0]);
		return -1;
	}
	if ((fp = fopen(argv[1], "a+")) == NULL)
	{
		perror("open for append error!\n");
		return -2;
	}

	for (int i = 0; i < sizeof(msg_sample) / sizeof(char *); i++)
	{
		backup_single_msg(msg_sample[i], 10000, 10001, fp);
	}

	rewind(fp);
	export_msg_from_to_sb(fp, stdout, 10000, 10001);
	return 0;
}