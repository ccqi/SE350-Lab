#ifndef _RTX_H
#define _RTX_H

#define RTX_ERR -1
#define NULL 0
#define RTX_SUCCESS 0

#define NUM_TEST_PROCS 2

#define HIGH		0
#define MEDIUM	1
#define LOW			2
#define LOWEST	3

#define MEMORY_BLOCK_SIZE 128
#define NUM_MEMORY_BLOCKS 1

typedef unsigned int U32;
typedef unsigned char U8;

// Process state
typedef enum {
	NEW,
	READY,
	BLOCK,
	WAIT,
	EXEC,
	INTERRUPT
} PROC_STATE;

// PCB
typedef struct {
	PROC_STATE state;
	U32 id;
	int priority;
	// Registers
	U32 pc;
	U32 *sp;
} PCB;

// Memory block
typedef struct {
	U32 *next;
} MEMORY_BLOCK;

// Initialization table item
typedef struct proc_init
{	
	int pid;
	int priority;
	int stack_size;
	void (*start_pc) ();
} PROC_INIT;

#endif
