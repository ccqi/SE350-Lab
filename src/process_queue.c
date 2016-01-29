#include "process_queue.h"

void process_enqueue(U32 *p_queue, PCB *pcb, int priority) {
	PROC_QUEUE *q = (PROC_QUEUE*) p_queue;
	int i;
	for (i = 1; i < priority; i++) {
		q = (PROC_QUEUE*) q->next;
	}
	pcb->next = q->PCB;
	q->PCB = (U32*) pcb;
}

void *process_dequeue(U32 *p_queue) {
	PROC_QUEUE *q = (PROC_QUEUE*) p_queue;
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		U32 *pcb = q->PCB;
		if (pcb != NULL) {
			q->PCB = ((PCB*) pcb)->next;
			return pcb;
		}
	}
	return NULL;
}

int process_empty(U32 *p_queue) {
	return 1;
}
