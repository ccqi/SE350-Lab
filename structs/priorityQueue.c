#include <stdlib.h>
#include <stdio.h>
#include "queue.c"

typedef struct {
	int priorityNum;
	queue **q;
} priorityQueue;

priorityQueue* priorityQueueCreate(int size, int priorityNum) {
	priorityQueue *pq = malloc(sizeof(priorityQueue));
	pq->priorityNum = priorityNum;
	pq->q = malloc(sizeof(queue*) * priorityNum);
	int i;
	for (i = 0; i < priorityNum; i++) {
		pq->q[i] = queueCreate(size);
	}
	return pq;
}

void priorityQueueEnqueue(priorityQueue *pq, void *value, int priority) {
	queueEnqueue(pq->q[priority], value);
}

void* priorityQueueDequeue(priorityQueue *pq) {
	int i;
	for (i = 0; i < pq->priorityNum; i++) {
		if (!queueEmpty(pq->q[i])) {
			return queueDequeue(pq->q[i]);
		}
	}
	return 0;
}

void* priorityQueuePeek(priorityQueue *pq) {
	int i;
	for (i = 0; i < pq->priorityNum; i++) {
		if (!queueEmpty(pq->q[i])) {
			return queuePeek(pq->q[i]);
		}
	}
	return 0;
}

int priorityQueueEmpty(priorityQueue *pq) {
	int i;
	for (i = 0; i < pq->priorityNum; i++) {
		if (!queueEmpty(pq->q[i])) {
			return 0;
		}
	}
	return 1;
}

void priorityQueueFree(priorityQueue *pq) {
	int i;
	for (i = 0; i < pq->priorityNum; i++) {
		queue *q = pq->q[i];
		queueFree(q);
	}
	free(pq->q);
	free(pq);
}

int main(void) {
	priorityQueue *pq = priorityQueueCreate(128, 5);
	int *a = malloc(sizeof(int));
	*a = 1;
	int *b = malloc(sizeof(int));;
	*b = 2;
	int *c = malloc(sizeof(int));;
	*c = 3;

	priorityQueueEnqueue(pq, a, 1);
	printf ("%d \n", *((int*)priorityQueuePeek(pq)));
	priorityQueueEnqueue(pq, b, 2);
	printf ("%d \n", *((int*)priorityQueuePeek(pq)));
	priorityQueueEnqueue(pq, c, 3);
	priorityQueueEnqueue(pq, a, 0);
	priorityQueueEnqueue(pq, b, 0);
	priorityQueueEnqueue(pq, c, 0);
	priorityQueueEnqueue(pq, c, 4);
	priorityQueueEnqueue(pq, b, 2);
	printf("%d \n", *((int*)priorityQueueDequeue(pq)));
	printf("%d \n", *((int*)priorityQueueDequeue(pq)));
	printf("%d \n", *((int*)priorityQueueDequeue(pq)));
	printf("%d \n", *((int*)priorityQueueDequeue(pq)));
	printf("%d \n", *((int*)priorityQueueDequeue(pq)));
	printf("Empty: %d \n", priorityQueueEmpty(pq));
	printf("%d \n", *((int*)priorityQueueDequeue(pq)));
	printf("Empty: %d \n", priorityQueueEmpty(pq));

	priorityQueueFree(pq);
	free(a);
	free(b);
	free(c);
	return 0;
}