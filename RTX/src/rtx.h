#ifndef _RTX_H
#define _RTX_H

#include "common.h"

#define RTX_ERR -1
#define NULL 0
#define RTX_OK 0

#define NUM_TEST_PROCS 6
#define NUM_PROCS 0x10

#define MEMORY_BLOCK_SIZE 128
#define NUM_MEMORY_BLOCKS 30

#define NUM_PROC_PRIORITY 5

// Process state
typedef enum {
	NEW,
	READY,
	BLOCK,
	WAIT,
	RUN,
	BLOCKED_ON_RECEIVE,
	WAITING_FOR_INTERRUPT
} PROC_STATE;

// PROC_QUEUE
typedef struct {
	U32 *first;
	U32 *last;
} PROC_QUEUE;

// Message
typedef struct {
#ifdef K_MSG_ENV
	void *next;
	int sPID;
	int rPID;
	int kdata[5];
#endif
	int type;
	char text[1];
#ifdef K_MSG_ENV
	int kbuffer[64];
	int expiry;
#endif
} MSG;

// MSG_QUEUE
typedef struct {
	MSG *first;
	MSG *last;
} MSG_QUEUE;

// PCB
typedef struct {
	PROC_STATE state;
	int id;
	int priority;
	// Registers
	U32 pc;
	U32 *sp;
	U32 *next;
	U32 *memory_block;
	MSG_QUEUE *message_queue;
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

/* IPC Management */
extern int k_send_message(int pid, void *p_msg);
#define send_message(pid, p_msg) _send_message((U32)k_send_message, pid, p_msg)
extern int _send_message(U32 p_func, int pid, void *p_msg) __SVC_0;

extern void *k_receive_message(int *p_pid);
#define receive_message(p_pid) _receive_message((U32)k_receive_message, p_pid)
extern void *_receive_message(U32 p_func, void *p_pid) __SVC_0;

/* Timing Service */
extern int k_delayed_send(int pid, void *p_msg, int delay);
#define delayed_send(pid, p_msg, delay) _delayed_send((U32)k_delayed_send, pid, p_msg, delay)
extern int _delayed_send(U32 p_func, int pid, void *p_msg, int delay) __SVC_0;  

#endif
