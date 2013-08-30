/*============================================================================*/
/*
 * @brief:   double list
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef _DLIST_H_
#define _DLIST_H_
#ifdef __CPLUSPLUS
extern "C" {
#endif /* __CPLUSPLUS */

#include "uOs_types.h"

/*Note: dlist is same to double list*/

struct dlist_node {
    struct dlist_node *prev;
    struct dlist_node *next;
};

struct dlist {
    struct dlist_node head;
    struct dlist_node tail;
};

STATIC INLINE void dlist_init(struct dlist *list)
{
    list->head.prev = NULL;    
    list->head.next = &list->tail;

    list->tail.prev = &list->head;
    list->tail.next = NULL;
}

STATIC INLINE uint8 dlist_empty(struct dlist *list)
{
    return (list->head.next == &list->tail) ? 1 : 0;
}

STATIC INLINE struct dlist_node *dlist_first(struct dlist *list)
{
    return dlist_empty(list) ? NULL : list->head.next;
}

STATIC INLINE struct dlist_node *dlist_last(struct dlist *list)
{
    return dlist_empty(list) ? NULL : list->tail.prev;
}

STATIC INLINE struct dlist_node *dlist_next(struct dlist *list, struct dlist_node *node)
{
    return (node->next == &list->tail) ? NULL : node->next;
}

STATIC INLINE struct dlist_node *dlist_prev(struct dlist *list, struct dlist_node *node)
{
    return ((node->prev = &list->head) ? NULL : node->prev);
}

STATIC INLINE void __dlist_add(struct dlist_node *new, struct dlist_node *prev, struct dlist_node *next)
{
    new->prev = prev;
    new->next = next;
    if (NULL != prev)
    {
        prev->next = new;
    }
    if (NULL != next)
    {
        next->prev = new;
    }
}

/* insert new entry after the specified node */
STATIC INLINE void dlist_add(struct dlist *list, struct dlist_node *new, struct dlist_node *node)
{
    if (NULL != node)
    {
        __dlist_add(new, node, node->next);
    }    
    else  /* insert new to last of list */
    {
        __dlist_add(new, list->tail.prev, &list->tail);
    }
}

STATIC INLINE void dlist_del(struct dlist *list, struct dlist_node *node)
{
    if (NULL != node->prev)
    {
        node->prev->next = node->next;
    }
    if (NULL != node->next)
    {
        node->next->prev = node->prev;    
    }
    
    node->prev = NULL;
    node->next = NULL;
}

/* insert new to last of list */
STATIC INLINE void dlist_append(struct dlist *list, struct dlist_node *new)
{
    dlist_add(list, new, NULL);    
}

#define contained_of(ptr, type, elem)  \
    (type *)((uint8 *)ptr - (uint8 *)&((type *)0)->elem)

#ifdef __CPLUSPLUS
}
#endif /* __CPLUSPLUS */
#endif /* _DLIST_H_ */
