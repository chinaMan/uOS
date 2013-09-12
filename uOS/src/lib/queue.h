/*============================================================================*/
/*
 * @brief:   queue
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef _QUEUE_H_
#define _QUEUE_H_
#ifdef __CPLUSPLUS
extern "C" {
#endif /* __CPLUSPLUS */

#include "uOs_types.h"

struct queue 
{
    uint8 size;
    uint8 in;
    uint8 out;
    void  **buf;
};

STATIC INLINE void queue_init(struct queue *queue, void **buf, uint8 size)
{
    queue->buf   = buf;
    queue->size  = size;
    queue->in    = 0;
    queue->out   = 0;
}

STATIC INLINE int queue_empty(struct queue *queue)
{
    return (queue->in == queue->out) ? 1 : 0;
}

STATIC INLINE int queue_full(struct queue *queue)
{
    return (queue->in == ((queue->out + 1) % queue->size)) ? 1 : 0;
}

STATIC INLINE void queue_en(struct queue *queue, void *data)
{
    queue->buf[queue->in] = data;
    queue->in = (queue->in + 1 + queue->size) % queue->size;
}

STATIC INLINE void queue_en_front(struct queue *queue, void *data)
{
    queue->out = (queue->out + queue->size - 1) % queue->size;
    queue->buf[queue->out] = data;
}

STATIC INLINE void* queue_de(struct queue *queue)
{
    void *data = NULL;

    data = queue->buf[queue->out];
    queue->out = (queue->out + 1 + queue->size) % queue->size;

    return data;
}

#ifdef __CPLUSPLUS
}
#endif /* __CPLUSPLUS */
#endif /* _QUEUE_H_ */
