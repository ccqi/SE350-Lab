#include "rtx.h"

#define RAM_END_ADDR 0x10008000

#ifdef DEBUG_0
extern unsigned int Image$$RW_IRAM1$$ZI$$Limit;
#else
unsigned int Image$$RW_IRAM1$$ZI$$Limit = 0;
#endif

extern PCB **gp_pcbs;
extern PROC_INIT g_proc_table[NUM_TEST_PROCS];

U32 *gp_stack;

U32 *p_heap;

void memory_init() {
	U8 *p_end = (U8*) &Image$$RW_IRAM1$$ZI$$Limit;
	int i;

	// 4 bytes Padding
	p_end += 4;

	// Allocate memory for pcb pointers
	gp_pcbs = (PCB**) p_end;
	p_end += NUM_TEST_PROCS * sizeof(PCB*);

	for (i = 0; i < NUM_TEST_PROCS; i++) {
		gp_pcbs[i] = (PCB*) p_end;
		p_end += sizeof(PCB);
	}

	// Prepare alloc_stack() for stack memory allocation 
	gp_stack = (U32*) RAM_END_ADDR;
	if ((U32) gp_stack & 0x04) {
		--gp_stack;
	}

	// Allocate memory for heap
	p_heap = (U32*) p_end;
	for (i = 0; i < NUM_MEMORY_BLOCKS; i++) {
		p_end += MEMORY_BLOCK_SIZE;
	}
}

U32 *alloc_stack(U32 size_b) 
{
	U32 *sp;
	sp = gp_stack; /* gp_stack is always 8 bytes aligned */
	
	/* update gp_stack */
	gp_stack = (U32 *)((U8 *)sp - size_b);
	
	/* 8 bytes alignement adjustment to exception stack frame */
	if ((U32)gp_stack & 0x04) {
		--gp_stack; 
	}
	return sp;
}

void *request_memory_block() {

}

int release_memory_block(void *memory_block) {
	return 0;
}
