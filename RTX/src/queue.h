#ifndef _QUEUE_H
#define _QUEUE_H

#include "rtx.h"

void queue_enqueue(QUEUE **queue, U32 *obj);
void *queue_dequeue(QUEUE **queue);
void *queue_peek(QUEUE **queue);

#endif
