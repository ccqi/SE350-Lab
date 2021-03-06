#ifndef _MEMORY_H
#define _MEMORY_H

#include "rtx.h"
#include "heap.h"

#define RAM_END_ADDR 0x10008000

extern unsigned int Image$$RW_IRAM1$$ZI$$Limit;

extern PCB **gp_pcbs;
extern PROC_QUEUE **gp_pcb_queue;
extern PROC_INIT g_proc_table[NUM_TEST_PROCS];
extern int release_processor(void);

void memory_init(void);
U32 *alloc_stack(U32 size_b);
void *request_memory_block(void);
int release_memory_block(void *memory_block);

#endif
