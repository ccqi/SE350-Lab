#include "memory.h"

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
	p_heap = NULL;
	for (i = 0; i < NUM_MEMORY_BLOCKS; i++) {
		MEMORY_BLOCK *block = (MEMORY_BLOCK*) p_end;
		heap_push(p_heap, block);
		p_end += sizeof(MEMORY_BLOCK*) + MEMORY_BLOCK_SIZE;
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
	U32 *block;
	while (heap_empty(p_heap)) { // CHECK IF BLOCK AVAILABLE FROM RELEASE_MEMORY_BLOCK
		// ADD PCB TO BLOCKED RESOURCE QUEUE
		// SET PROCSS STATE TO BLOCK
		release_processor();
	}
	block = heap_pop(p_heap);
	return block;
}

int release_memory_block(void *memory_block) {
	if (memory_block == NULL) {
		return RTX_ERR;
	}

	if (0) { // BLOCKED RESOURCE QUEUE
		// GIVE BLOCK TO BLOCKED RESOURCE
	} else {
		heap_push(p_heap, memory_block);
	}
	return RTX_OK;
}
