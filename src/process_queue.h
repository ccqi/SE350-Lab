#ifndef _PROCESS_QUEUE_H
#define _PROCESS_QUEUE_H

#include "rtx.h"

void process_enqueue(U32 *p_queue, PCB *pcb, int priority);
void *process_dequeue(U32 *p_queue);
int process_empty(U32 *p_queue);

#endif
