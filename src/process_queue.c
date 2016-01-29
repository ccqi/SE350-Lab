#include "process_queue.h"

void process_enqueue(PROC_QUEUE **p_queue, PCB *pcb, int priority) {
	PROC_QUEUE *q = p_queue[priority];
	pcb->next = q->PCB;
	q->PCB = (U32*) pcb;
}

void *process_dequeue(PROC_QUEUE **p_queue) {
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_queue[i];
		U32 *pcb = q->PCB;
		if (pcb != NULL) {
			q->PCB = ((PCB*) pcb)->next;
			return pcb;
		}
	}
	return NULL;
}

void *process_find(PROC_QUEUE **p_queue, int pid) {
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_queue[i];
		PCB *pcb = (PCB*) q->PCB;
		while (pcb != NULL) {
			if (pcb->id == pid) {
				return pcb;
			}
			pcb = (PCB*) pcb->next;
		}
	}
	return NULL;
}

void *process_remove(PROC_QUEUE **p_queue, int pid) {
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_queue[i];
		PCB *pcb = (PCB*) q->PCB;

		if (pcb != NULL && pcb->id == pid) {
			q->PCB = pcb->next;
			return pcb;
		} else {
			while (pcb != NULL) {
				PCB *prev = pcb;
				pcb = (PCB*) pcb->next;
				if (pcb->id == pid) {
					prev->next = pcb->next;
					return pcb;
				}
				prev = pcb;
				pcb = (PCB*) pcb->next;
			}
		}
	}
	return NULL;
}
