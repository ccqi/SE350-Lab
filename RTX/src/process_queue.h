#ifndef _PROCESS_QUEUE_H
#define _PROCESS_QUEUE_H

#include "rtx.h"
#include "uart_polling.h"

void process_enqueue(PROC_QUEUE **p_queue, PCB *pcb, int priority);
void *process_dequeue(PROC_QUEUE **p_queue);
void *process_find(PROC_QUEUE **p_queue, int pid);
void *process_remove(PROC_QUEUE **p_queue, int pid);
void *process_peek(PROC_QUEUE **p_queue);
void *process_peek_ready(PROC_QUEUE **p_queue);
void *process_peek_block(PROC_QUEUE **p_queue);
void process_print_ready(PROC_QUEUE **p_queue);
void process_print_blocked(PROC_QUEUE **p_queue);
void process_print_blocked_on_receive(PROC_QUEUE **p_queue);

#endif
