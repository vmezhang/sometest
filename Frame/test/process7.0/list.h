/*
 * =====================================================================================
 *
 *       Filename:  list.h
 *
 *    Description:  双向循环链表
 *
 *        Version:  1.0
 *        Created:  2015年05月13日 10时03分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef __LIST_H__
#define __LIST_H__

#ifndef __ASSEMBLER__

#include <stdbool.h>

#include "types.h"

struct list_entry {
    struct list_entry *prev, *next;
};

typedef struct list_entry list_entry_t;

static inline void list_init(list_entry_t *elm) __attribute__((always_inline));
static inline void list_add(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
static inline void list_add_before(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
static inline void list_add_after(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
static inline void list_del(list_entry_t *listelm) __attribute__((always_inline));
static inline void list_del_init(list_entry_t *listelm) __attribute__((always_inline));
static inline bool list_empty(list_entry_t *list) __attribute__((always_inline));
static inline list_entry_t *list_next(list_entry_t *listelm) __attribute__((always_inline));
static inline list_entry_t *list_prev(list_entry_t *listelm) __attribute__((always_inline));

static inline void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) __attribute__((always_inline));
static inline void __list_del(list_entry_t *prev, list_entry_t *next) __attribute__((always_inline));

//初始化一个双向循环链表
static inline void list_init(list_entry_t *elm)
 {
    elm->prev = elm->next = elm;
}

//在一个节点之后增加一个新节点
static inline void list_add(list_entry_t *listelm, list_entry_t *elm)
 {
    list_add_after(listelm, elm);
}

//在一个节点之前增加新节点
static inline void list_add_before(list_entry_t *listelm, list_entry_t *elm) 
{
    __list_add(elm, listelm->prev, listelm);
}

static inline void list_add_after(list_entry_t *listelm, list_entry_t *elm) 
{
    __list_add(elm, listelm, listelm->next);
}

//删除指定的节点
static inline void list_del(list_entry_t *listelm) 
{
    __list_del(listelm->prev, listelm->next);
}


static inline void list_del_init(list_entry_t *listelm) 
{
    list_del(listelm);
    list_init(listelm);
}

//判断双向循环链表是否为空
static inline bool list_empty(list_entry_t *list) 
{
    return list->next == list;
}

//得到当前节点的下一个节点
static list_entry_t * list_next(list_entry_t *listelm)
{
    return listelm->next;
}

//得到当前节点的下一个节点
static list_entry_t * list_prev(list_entry_t *listelm)
{
    return listelm->prev;
}

static inline void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) 
{
    prev->next = next->prev = elm;
    elm->next = next;
    elm->prev = prev;
}

static inline void __list_del(list_entry_t *prev, list_entry_t *next) 
{
    prev->next = next;
    next->prev = prev;
}

#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({\
			const typeof(((type *)0)->member)*_mptr = (ptr);\
			(type *)((char *)_mptr - offsetof(type, member));})

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#endif /* !__ASSEMBLER__ */

#endif /* !__LIST_H__ */

