#pragma once
#ifndef LIST_H
#define LIST_H


struct list_head 
{
	struct list_head  *next, *prev;
};


/*ºê*/
#define LIST_POSITION ((void *) 0x0)

#define LIST_HEAD_INIT(name)  { &(name), &(name)}
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)
#define INIT_LIST_HEAD(ptr) \
	do{(ptr)->next=(ptr);(ptr)->prev=(ptr); \
} while(0)


#define list_for_each(pos, head) \
	for(pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
	for(pos = (head)->prev; pos != (head); pos = pos->prev)

/*

size_t

32 -> typedef unsinged int size_t;
64 -> typedef unsinged long size_t;

*/
#define offsetof(t, m) \
	((size_t)(&((t *)0) -> m))


#define container_of(ptr, type, member) \
	(((char *)(ptr)) - offsetof(type, member))

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)



void __list_add(struct list_head *node, struct list_head *prev, struct list_head *next);
void __list_del(struct list_head *prev, struct list_head *next);
void list_add(struct list_head *node, struct list_head *head);
void list_del(struct list_head *entry);
void list_del_init(struct list_head *entry);
void list_add_tail(struct list_head *node, struct list_head *head);
int list_empty(const struct list_head *head);

#endif // !LIST_H
