#include "process_queue.h"

#ifdef DEBUG_0
#include "printf.h"
#endif

void process_enqueue(PROC_QUEUE **p_queue, PCB *pcb, int priority) {
	PROC_QUEUE *q = p_queue[priority];
	pcb->next = NULL;
	if (q->last == NULL) {
		q->first = q->last = (U32*) pcb;
	} else {
		((PCB*) q->last)->next = (U32*) pcb;
		q->last = (U32*) pcb;
	}
}

void *process_dequeue(PROC_QUEUE **p_queue) {
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_queue[i];
		U32 *pcb = q->first;
		if (pcb != NULL) {
			q->first = ((PCB*) pcb)->next;
			if (q->first == NULL) {
				q->last = NULL;
			}
			return pcb;
		}
	}
	return NULL;
}

void *process_find(PROC_QUEUE **p_queue, int pid) {
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_queue[i];
		PCB *pcb = (PCB*) q->first;
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
		PCB *pcb = (PCB*) q->first;

		if (pcb != NULL) {
			if (pcb->id == pid) {
				q->first = pcb->next;
				if (q->first == NULL) {
					q->last = NULL;
				}
				return pcb;
			} else {
				PCB *prev = pcb;
				pcb = (PCB*) pcb->next;
				while (pcb != NULL) {
					if (pcb->id == pid) {
						prev->next = pcb->next;
						if (prev->next == NULL) {
							q->last = (U32*) prev;
						}
						return pcb;
					}
					prev = pcb;
					pcb = (PCB*) pcb->next;
				}
			}
		}
	}
	return NULL;
}

void *process_peek(PROC_QUEUE **p_queue) {
	int i;
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		PROC_QUEUE *q = p_queue[i];
		if (q->first != NULL) {
			return q->first;
		}
	}
	return NULL;
}
