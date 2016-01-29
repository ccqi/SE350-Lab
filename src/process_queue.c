#include "process_queue.h"

void process_enqueue(PROC_QUEUE **p_queue, PCB *pcb, int priority) {
	PROC_QUEUE **p_q = (PROC_QUEUE**) p_queue;
	PROC_QUEUE *q = p_q[priority];
	pcb->next = q->PCB;
	q->PCB = (U32*) pcb;
}

void *process_dequeue(PROC_QUEUE **p_queue) {
	PROC_QUEUE **p_q = (PROC_QUEUE**) p_queue;
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_q[i];
		U32 *pcb = q->PCB;
		if (pcb != NULL) {
			q->PCB = ((PCB*) pcb)->next;
			return pcb;
		}
	}
	return NULL;
}
