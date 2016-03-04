#include "message_queue.h"

void message_queue_enqueue(MSG_QUEUE *queue, MSG *msg) {
	msg->next = NULL;
	if (queue->last == NULL) {
		queue->first = queue->last = msg;
	} else {
		queue->last->next = msg;
		queue->last = msg;
	}
}

void *message_queue_dequeue(MSG_QUEUE *queue) {
	MSG *msg = queue->first;
	if (msg == NULL) {
		return NULL;
	} else {
		queue->first = msg->next;
		msg->next = NULL;
		if (queue->first == NULL) {
			queue->last = NULL;
		}
		return msg;
	}
}

void *message_queue_peek(MSG_QUEUE *queue) {
	return queue->first;
}

int message_queue_empty(MSG_QUEUE *queue) {
	if (message_queue_peek(queue) == NULL) {
		return 1;
	}
	return 0;
}
