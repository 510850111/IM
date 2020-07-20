#include "im_client.h"

struct whoami myself;

int init_user_struct(MYSELF *m)
{
	memset(m, 0, sizeof(*m));

	m->w_id = -1;
	m->w_sockfd = -1;
	m->w_cstst = INIT_CSTAT;
	m->w_mstat = MSG_SNDING;

	INIT_LIST_HEAD(&myself.w_flisthd);
	
	return 0;
}

int user_list_add(FRND_ST *fs, short cnt)
{
	int i = 0;
	CLIENT_FRND *cf, *cp;
	struct list_head *pos, *head = &myself.w_flisthd;

	for (i = 0; i < cnt; i++)
	{
		printf("client: user %d , name=%s, id=%d, state=%d\n",
			i, fs[i].fs_name, fs[i].fs_id, fs[i].fs_stat);
	}

	for (i = 0; i < cnt; i++)
	{
		list_for_each(pos, head)
		{
			cp = (CLIENT_FRND *)list_entry(pos, CLIENT_FRND, c_node);
			if (cp->c_id == fs[i].fs_id)
			{
				strcpy(cp->c_name, fs[i].fs_name);
				cp->c_stat = fs[i].fs_stat;
				break;
			}
		}

		if (pos != head)
		{
			printf("client: user in the friend list, update status!\n");
			break;
		}

		cf = (CLIENT_FRND *)malloc(sizeof(*cf));
		if (cf == NULL)
		{
			printf("client: malloc failed!\n");
			continue;
		}

		printf("client: user NOT int the friend list, add this user now.\n");
		memset(cf, 0, sizeof(*cf));
		strncpy(cf->c_name, fs[i].fs_name, MAXNAME_LEN-1);
		cf->c_id = fs[i].fs_id;
		cf->c_stat = fs[i].fs_name;

		list_add(&(cf->c_node), head);
	}

	return 0;
}

int user_list_del(FRND_ST *fs, short cnt)
{
	CLIENT_FRND *cp;
	struct list_head *pos = NULL, *head = &myself.w_flisthd;

	for (int i = 0; i < cnt; i++)
	{
		list_for_each(pos, head)
		{
			cp = (CLIENT_FRND *)list_entry(pos, CLIENT_FRND, c_node);
			if (cp->c_id == fs[i].fs_id)
			{
				break;
			}
		}

		if (pos == head)
		{
			continue;
		}

		cp = (CLIENT_FRND *)list_entry(pos, CLIENT_FRND, c_node);
		list_del(&(cp->c_node));
		free(cp);
	}
	return 0;
}


int user_list_update(FRND_ST *fs, short cnt)
{
	for (int i = 0; i < cnt; i++)
	{
		printf("client: user %d, name=%s, id=%d,stat=%d\n",
			i,fs[i].fs_name, fs[i].fs_id, fs[i].fs_stat);
	}

	return 0;
}

int user_list_output(FRND_ST *fs, short cnt)
{
	for (int i = 0; i < cnt; i++)
	{
		printf("client: (alist msg)-> user=%d, name=%s, id=%d, stat=%d\n",
			i, fs[i].fs_name, fs[i].fs_id, fs[i].fs_stat);
	}
	return 0;
}

