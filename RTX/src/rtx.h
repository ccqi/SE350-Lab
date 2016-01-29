#ifndef _RTX_H
#define _RTX_H

#define RTX_ERR -1
#define NULL 0
#define RTX_OK 0

#define NUM_TEST_PROCS 4
#define NUM_PROCS 5

#define HIGH		0
#define MEDIUM	1
#define LOW			2
#define LOWEST	3

#define MEMORY_BLOCK_SIZE 128
#define NUM_MEMORY_BLOCKS 4

#define NUM_PROC_PRIORITY 5

typedef unsigned int U32;
typedef unsigned char U8;

// Process state
typedef enum {
	NEW,
	READY,
	BLOCK,
	WAIT,
	RUN,
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
	U32 *next;
} PCB;

// PROC_QUEUE
typedef struct {
	U32 *first;
	U32 *last;
} PROC_QUEUE;

// Memory block
typedef struct {
	U32 *next;
} MEMORY_BLOCK;

// Initialization table item
typedef struct proc_init
{	
	U32 pid;
	int priority;
	int stack_size;
	void (*start_pc) ();
} PROC_INIT;

/* ----- RTX User API ----- */
#define __SVC_0  __svc_indirect(0)

extern void k_rtx_init(void);
#define rtx_init() _rtx_init((U32)k_rtx_init)
extern void __SVC_0 _rtx_init(U32 p_func);

extern int k_release_processor(void);
#define release_processor() _release_processor((U32)k_release_processor)
extern int __SVC_0 _release_processor(U32 p_func);

extern void *k_request_memory_block(void);
#define request_memory_block() _request_memory_block((U32)k_request_memory_block)
extern void *_request_memory_block(U32 p_func) __SVC_0;
/* __SVC_0 can also be put at the end of the function declaration */

extern int k_release_memory_block(void *);
#define release_memory_block(p_mem_blk) _release_memory_block((U32)k_release_memory_block, p_mem_blk)
extern int _release_memory_block(U32 p_func, void *p_mem_blk) __SVC_0;

extern int k_set_process_priority(int, int);
#define set_process_priority(process_id, priority) _set_process_priority((U32)k_set_process_priority, process_id, priority)
extern int _set_process_priority(U32 p_func, int process_id, int priority) __SVC_0;

extern int k_get_process_priority(int);
#define get_process_priority(process_id) _get_process_priority((U32)k_get_process_priority, process_id)
extern int _get_process_priority(U32 p_func, int process_id) __SVC_0;

#endif
