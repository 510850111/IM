#include "im_client.h"

/*
	����һ���½ڵ�
	@node: �ڵ�ָ��
	@prev: ָ����һ���ڵ��ָ��
	@next: ָ����һ���ڵ��ָ��
*/
void __list_add(struct list_head *node, struct list_head *prev, struct list_head *next)
{
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/*ͷ������*/
void list_add(struct list_head *node, struct list_head *head)
{
	__list_add(node, head, head->next);
}
/*β������*/
void list_add_tail(struct list_head *node, struct list_head *head)
{
	__list_add(node, head->prev, head);
}
/*�ڵ�ɾ��*/
void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POSITION;
	entry->prev = LIST_POSITION;
}
void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/*�ж��Ƿ�Ϊ������*/
int list_empty(const struct list_head *head)
{
	return head->next == head;
}