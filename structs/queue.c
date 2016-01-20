#include "node.c"
#include <string.h>

typedef struct {
	int nodeSize;
	node *head;
	node *tail;
} queue;

queue* queueCreate(int size) {
	queue *q = malloc(sizeof(queue));
	q->nodeSize = size;
	q->head = q->tail = NULL;
	return q;
}

void queueEnqueue(queue *q, void *value) {
	node *newNode = malloc(sizeof(node));
	newNode->next = NULL;
	newNode->value = value;
	
	if (q->head) {
		q->tail->next = newNode;
		q->tail = newNode;
	} else {
		q->head = q->tail = newNode; 
	}
}

void* queueDequeue(queue *q) {
	node *head = q->head;
	if (q->head->next) {
		q->head = q->head->next;
	} else{
		q->head = q->tail = NULL;
	}
	return head->value;
}

int main(void) {
	queue *q = queueCreate(128);
	int *a = malloc(sizeof(int));
	*a = 1;
	int *b = malloc(sizeof(int));;
	*b = 2;
	int *c = malloc(sizeof(int));;
	*c = 3;
	queueEnqueue(q, a);
	printf ("%d \n", *((int*)q->head->value));
	queueEnqueue(q, b);
	printf ("%d \n", *((int*)q->head->value));
	queueEnqueue(q, c);
	printf ("%d \n", *((int*)q->head->value));
	printf("%d \n", *((int*)queueDequeue(q)));
	printf("%d \n", *((int*)queueDequeue(q)));
	printf("%d \n", *((int*)queueDequeue(q)));
	return 0;
}
