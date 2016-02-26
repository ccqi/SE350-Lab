#ifndef _MESSAGE_QUEUE_H
#define _MESSAGE_QUEUE_H

#include "rtx.h"

void message_queue_enqueue(MSG_QUEUE *queue, MSG *msg);
void *message_queue_dequeue(MSG_QUEUE *queue);
void *message_queue_peek(MSG_QUEUE *queue);
int message_queue_empty(MSG_QUEUE *queue);

#endif
