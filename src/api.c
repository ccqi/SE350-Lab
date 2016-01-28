#include "rtx.h"
#include "stack.c"

typedef enum {
	NEW,
	READY,
	RUN
} process_state;

typedef struct {
	process_state state;
	U32 id;
	int priority;
	// Registers
	U32 pc;
	U32 *sp;
} PCB;

int set_process_priority(int process_id, int priority) {
	return 0;
}

int get_process_priority(int process_id) {

	return -1;
}

int release_processor() {
	return 0;
}

///////////////////////////////////////////////////////////////////////
// Memory
///////////////////////////////////////////////////////////////////////
#define RAM_END_ADDR 0x10008000

extern unsigned int Image$$RW_IRAM1$$ZI$$Limit;

U32 *gp_stack;



void memory_init() {
	U8 *p_end = (U8*) &Image$$RW_IRAM1$$ZI$$Limit;
	int i;

	// 4 bytes Padding
	p_end += 4;

	// Memory for pcb pointers
	gp_pcbs = (PCB**) p_end;
	p_end += NUM_TEST_PROCS * sizeof(PCB*);

	// Set gp_stack to high address
	gp_stack = (U32*) RAM_END_ADDR;
	if ((U32) gp_stack & 0x04) {
		--gp_stack;
	}
	// Allocate memory for heap

}

void *request_memory_block() {

}

int release_memory_block(void *memory_block) {
	return 0;
}
