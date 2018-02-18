#ifndef _LIST_H
#define _LIST_H

#include "types.h"

typedef struct s_LIST_H {
	struct s_LIST_H *prev;
	struct s_LIST_H *next;
}list_h_t;

static inline void list_init(list_h_t* list)
{
    list->prev=list;
    list->next=list;
    return;
}

static inline void __list_del(list_h_t* prev, list_h_t* next)
{
    next->prev = prev;
    prev->next = next;
    return;
}

static inline void __list_add(list_h_t *new,list_h_t *prev,list_h_t *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
    return;
}

static inline void list_add(list_h_t *new, list_h_t *head)
{
         __list_add(new, head, head->next);
    return;
}

static inline void list_add_tail(list_h_t* new,list_h_t* head)
{
        __list_add(new, head->prev, head);
    return; 
}


static inline void __list_del_entry(list_h_t* entry)
{
    __list_del(entry->prev, entry->next);
    return;
}

static inline void list_del(list_h_t* entry)
{
    __list_del(entry->prev, entry->next);
    list_init(entry);
    return;
}

static inline void list_move(list_h_t* list,list_h_t* head)
{
         list_del(list);
         list_add(list, head);
         return;
}
static inline void list_move_tail(list_h_t* list,list_h_t* head)
{
         list_del(list);
         list_add_tail(list, head);
         return;
}


static inline bool_t list_is_empty(const list_h_t* head)
{
    if(head->next == head)
    {
        return TRUE;
    }
    return FALSE;
}

static inline bool_t list_is_empty_careful(const list_h_t* head)
{
    list_h_t* next = head->next;
    if(next == head && next == head->prev)
    {
        return TRUE;
    }
    return FALSE;
}


#define list_for_each(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next)
#define list_for_each_head_dell(pos, head) for (pos = (head)->next; pos != (head); pos = (head)->next)

#define list_entry(ptr, type, member) \
((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif /*End of _LIST_H*/
