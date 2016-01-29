#ifndef _PROCESS_QUEUE_H
#define _PROCESS_QUEUE_H

#include "rtx.h"

void process_enqueue(PROC_QUEUE **p_queue, PCB *pcb, int priority);
void *process_dequeue(PROC_QUEUE **p_queue);

#endif
