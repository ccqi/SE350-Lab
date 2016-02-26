#include "queue.h"

void queue_enqueue(QUEUE **queue, U32 *obj) {
	if (queue->last == NULL) {
		queue->first = queue->last = obj;
	} else {
		queue->last->next = obj;
		queue->last = obj;
	}
}

void *queue_dequeue(QUEUE **queue) {
	void *obj = queue->first;
	if (obj == NULL) {
		return NULL;
	} else {
		queue->first = obj->next;
		if (queue->first == NULL) {
			queue->last = NULL;
		}
		return obj;
	}
}

void *queue_peek(QUEUE **queue) {
	return queue->first;
}