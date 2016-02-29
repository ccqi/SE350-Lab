#include "memory.h"

#ifdef DEBUG_0
#include "printf.h"
#endif

U32 *gp_stack;

U32 *p_heap;

void memory_init() {
	U8 *p_end = (U8*) &Image$$RW_IRAM1$$ZI$$Limit;
	int i;

	// 4 bytes Padding
	p_end += 4;

	// Allocate memory for pcb pointers
	gp_pcbs = (PCB**) p_end;
	p_end += NUM_PROCS * sizeof(PCB*);
	for (i = 0; i < NUM_PROCS; i++) {
		gp_pcbs[i] = (PCB*) p_end;
		p_end += sizeof(PCB);

		gp_pcbs[i]->message_queue = (MSG_QUEUE*) p_end;
		gp_pcbs[i]->message_queue->first = NULL;
		gp_pcbs[i]->message_queue->last = NULL;
		p_end += sizeof(MSG_QUEUE);
	}

	// Timeout queue
	gp_timeout_queue = (MSG_QUEUE*) p_end;
	gp_timeout_queue->first = NULL;
	gp_timeout_queue->last = NULL;
	p_end += sizeof(MSG_QUEUE);

	// Input buffer from uart
	gp_input_buffer = (char*) p_end;
	p_end += 10 * sizeof(char);

	// Allocate memory for priority queue
	gp_pcb_queue = (PROC_QUEUE**) p_end;
	p_end += NUM_PROC_PRIORITY * sizeof(PROC_QUEUE*);
	for (i = 0; i < NUM_PROC_PRIORITY; i++) {
		gp_pcb_queue[i] = (PROC_QUEUE*) p_end;
		gp_pcb_queue[i]->first = NULL;
		gp_pcb_queue[i]->last = NULL;
		p_end += sizeof(PROC_QUEUE);
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
		heap_push(&p_heap, block);
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

void *k_request_memory_block() {
	U32 *block = NULL;

	while (gp_current_process->memory_block == NULL && heap_empty(&p_heap)) {
		// Block current process until a block is free
		gp_current_process->state = BLOCK;
		k_release_processor();
	}
	if (gp_current_process->memory_block != NULL) { // Unblocked by release_memory_block
		block = gp_current_process->memory_block;
		gp_current_process->memory_block = NULL;
	} else { // Memory available in heap
		block = heap_pop(&p_heap);
	}
	return block;
}

int k_release_memory_block(void *memory_block) {
	PCB* blocked_process;

	if (memory_block == NULL) {
		return RTX_ERR;
	}
	
	// Check for blocked resources
	blocked_process = (PCB*) process_peek_block(gp_pcb_queue);
	if (blocked_process != NULL) {
		// Give block to blocked resource
		blocked_process->memory_block = (U32*) memory_block;
		blocked_process->state = READY;
		// Switch process if higher priority unblocked
		k_release_processor();
	} else {
		// Push back onto heap
		heap_push(&p_heap, memory_block);
	}
	return RTX_OK;
}
