#include "im_serv.h"

/*表示所有用户的结构体数组
数组索引与id相对应

1001 all_users[1]

*/
IM_USER all_users[MAX_USER_NUM];

int user_id = CONF_LOW_ID;
pthread_mutex_t uid_lock = PTHREAD_MUTEX_INITIALIZER;

int reg_user_num = 0;
pthread_mutex_t reg_user_lock = PTHREAD_MUTEX_INITIALIZER;

void reg_user_num_inc()
{
	pthread_mutex_lock(&reg_user_lock);
	reg_user_num++;
	pthread_mutex_unlock(&reg_user_lock);
}

int get_next_uid()
{
	pthread_mutex_lock(&uid_lock);
	user_id++;
	pthread_mutex_unlock(&uid_lock);

	return user_id;
}


int init_all_usr_struct(int argc, char **argv)
{
	IM_USER *u = all_users;

	memset(all_users, 0, sizeof(IM_USER)*MAX_USER_NUM);
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		u->u_id = CONF_LOW_ID + 1;
		u->u_sckfd = -1;
		INIT_LIST_HEAD(&u->u_frndhd);

		u++;
	}
	return 0;
}

void usr_fill_frndst(FRND_ST *fs, int cnt, IM_USER *u)
{
	USR_FRND *pos;
	IM_USER *p;

	uflist_for_each_entry(pos, &u->u_frndhd, f_node)
	{
		p = &all_users[pos->f_id - CONF_LOW_ID];
		memset(fs, 0, sizeof(*fs));
		strncpy(fs->fs_name, p->u_name, MAXNAME_LEN - 1);
		fs->fs_id = p->u_id;
		fs->fs_stat = p->u_stat;
		fs++;
		cnt--;
		if (!cnt)
		{
			break;
		}
	}
}

void get_online_users(FRND_ST *fs, int *cnt)
{
	IM_USER *p = all_users;
	int n = *cnt;

	memset(fs, 0, sizeof(*fs) *(*cnt));
	for (int i = 0; n > 0 && i < MAX_USER_NUM; i++, p++)
	{
		if (p->u_stat & USER_LOGIN)
		{
			fs->fs_id = p->u_id;
			strncpy(fs->fs_name, p->u_name, MAXNAME_LEN - 1);
			fs->fs_stat = p->u_stat;

			fs++;
			n--;
		}
	}

	*cnt = *cnt - n;
}

USR_FRND * usr_friend_find(IM_USER *u, int uid)
{
	USR_FRND *pos;
	//printf("serv: entry usr_friend_find, uid=%d.\n", uid);
	uflist_for_each_entry(pos, &u->u_frndhd, f_node)
	{
		if (pos->f_id == uid)
		{
			printf("serv: user find! id=%d\n", pos->f_id);
			return pos;
		}
	}
	printf("serv: user NOT found!");
	return NULL;
}

int usr_friend_add(IM_USER *u, int uid)
{
	USR_FRND *uf;
	printf("serv: entry usr_friend_add, uid=%d\n",uid);
	uf = usr_friend_find(u, uid);
	printf("serv: usr_friend_add\n");
	if (uf == NULL)
	{
		uf = (USR_FRND *)malloc(sizeof(*uf));
		if (uf == NULL)
		{
			return -1;
		}

		uf->f_id = uid;
		
		list_add(&uf->f_node, &u->u_frndhd);
		printf("serv: usr_friend_add ok!\n");
		u->u_frndcnt++;

		return 0;
	}

	return -1;	
}

int usr_friend_del(IM_USER *u, int uid)
{
	USR_FRND *uf;
	uf = usr_friend_find(u, uid);
	if (uf != NULL)
	{
		list_del(&uf->f_node);
		u->u_frndcnt--;
		free(uf);
		printf("serv: user:%d del OK, memory free.\n",uid);
		return 0;
	}

	return -1;
}

void usr_logout_exit(int uid)
{
	IM_USER *p;

	p = &all_users[uid - CONF_LOW_ID];
	p->u_stat &= ~USER_LOGIN;
	printf("serv: logout uid=%d, stat=%x\n", uid, p->u_stat);

	closesocket(p->u_frndcnt);
}