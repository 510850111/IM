#include "im_client.h"

/*
	插入一个新节点
	@node: 节点指针
	@prev: 指向上一个节点的指针
	@next: 指向下一个节点的指针
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

/*头部插入*/
void list_add(struct list_head *node, struct list_head *head)
{
	__list_add(node, head, head->next);
}
/*尾部插入*/
void list_add_tail(struct list_head *node, struct list_head *head)
{
	__list_add(node, head->prev, head);
}
/*节点删除*/
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

/*判断是否为空链表*/
int list_empty(const struct list_head *head)
{
	return head->next == head;
}