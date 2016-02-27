#ifndef _PROCESS_H
#define _PROCESS_H

#include "rtx.h"
#include "process_queue.h"

#define INITIAL_xPSR 0x01000000

extern PROC_INIT g_test_procs[NUM_TEST_PROCS];
extern PROC_INIT g_i_procs[2];

extern void set_test_procs(void);
extern void set_i_procs(void);

extern void __rte(void);
extern U32 *alloc_stack(U32 size_b);

void process_init(void);
int k_set_process_priority(int process_id, int priority);
int k_get_process_priority(int process_id);
int k_release_processor(void);

PCB *get_timer_process(void);
PCB *get_uart_process(void);

#endif
