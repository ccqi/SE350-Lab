#include "message_queue.h"

void queue_enqueue(QUEUE **queue, MESSAGE_QUEUE *msg) {
	msg->next = NULL;
	if (queue->last == NULL) {
		queue->first = queue->last = msg;
	} else {
		queue->last->next = msg;
		queue->last = msg;
	}
}

void *queue_dequeue(QUEUE **queue) {
	MESSAGE_QUEUE *msg = queue->first;
	if (msg == NULL) {
		return NULL;
	} else {
		msg->next = NULL;
		queue->first = msg->next;
		if (queue->first == NULL) {
			queue->last = NULL;
		}
		return msg;
	}
}

void *queue_peek(QUEUE **queue) {
	return queue->first;
}