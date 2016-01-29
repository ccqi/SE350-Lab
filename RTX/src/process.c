#include "process.h"
#include <LPC17xx.h>
#include <system_LPC17xx.h>

#ifdef DEBUG_0
#include "printf.h"
#endif

PCB **gp_pcbs;
PCB *gp_current_process = NULL;

U32 g_switch_flag = 0;

PROC_QUEUE **gp_pcb_queue;

PROC_INIT g_proc_table[NUM_PROCS];

PCB **gp_pcbs_ready;

void null_process(void);
void set_null_proc(void);

void process_init() {
	U32 *sp;
	int i;

	// Initialization table
	set_test_procs();
	for (i = 1; i <= NUM_TEST_PROCS; i++) {
		g_proc_table[i].pid = g_test_procs[i - 1].pid;
		g_proc_table[i].stack_size = g_test_procs[i - 1].stack_size;
		g_proc_table[i].start_pc = g_test_procs[i - 1].start_pc;
		g_proc_table[i].priority = g_test_procs[i - 1].priority;
	}

	// Null process
	set_null_proc();

	// Initialize exception stack frame (initial context) for each process
	for (i = 0; i < NUM_PROCS; i++) {
		int j;
		gp_pcbs[i]->id = g_proc_table[i].pid;
		gp_pcbs[i]->state = NEW;

		sp = alloc_stack(g_proc_table[i].stack_size);
		*(--sp) = INITIAL_xPSR;      // user process initial xPSR  
		*(--sp) = (U32)((g_proc_table[i]).start_pc); // PC contains the entry point of the process
		for (j = 0; j < 6; j++) { // R0-R3, R12 are cleared with 0
			*(--sp) = 0x0;
		}
		gp_pcbs[i]->sp = sp;

		// Add to priority queue
		gp_pcbs[i]->priority = g_proc_table[i].priority;
		process_enqueue(gp_pcb_queue, gp_pcbs[i], gp_pcbs[i]->priority);
	}

	#ifdef DEBUG_0
	printf("gp_pcbs[1]->priority = 0x%x \n", gp_pcbs[1]->priority);
	printf("gp_pcbs[1]->priority = 0x%x \n", k_get_process_priority(gp_pcbs[1]->id));
	printf("gp_pcbs[1]->priority = 0x%x \n", gp_pcbs[1]->priority);
	k_set_process_priority(gp_pcbs[1]->id, HIGH);
	printf("gp_pcbs[1]->priority = 0x%x \n", k_get_process_priority(gp_pcbs[1]->id));
	printf("gp_pcbs[1]->priority = 0x%x \n", gp_pcbs[1]->priority);
	k_set_process_priority(gp_pcbs[1]->id, MEDIUM);
	printf("gp_pcbs[1]->priority = 0x%x \n", k_get_process_priority(gp_pcbs[1]->id));
	printf("gp_pcbs[1]->priority = 0x%x \n", gp_pcbs[1]->priority);
	k_set_process_priority(gp_pcbs[1]->id, LOW);
	printf("gp_pcbs[1]->priority = 0x%x \n", k_get_process_priority(gp_pcbs[1]->id));
	printf("gp_pcbs[2]->priority = 0x%x \n", gp_pcbs[2]->priority);
	printf("gp_pcbs[2]->priority = 0x%x \n", gp_pcbs[2]->priority);
	#endif
}

int k_set_process_priority(int process_id, int priority) {
	PCB *pcb = (PCB*) process_remove(gp_pcb_queue, process_id);
	
	if (pcb == NULL)
		return RTX_ERR;

	pcb->priority = priority;
	process_enqueue(gp_pcb_queue, pcb, priority);
	return RTX_OK;
}

int k_get_process_priority(int process_id) {
	PCB* pcb = process_find(gp_pcb_queue, process_id);

	if (pcb != NULL) {
		return pcb->priority;
	}
	return RTX_ERR;
}

// todo
PCB *scheduler(void) {
	if (gp_current_process == NULL) {
		gp_current_process = gp_pcbs[0]; 
		return gp_pcbs[0];
	}

	if (gp_current_process == gp_pcbs[0]) {
		return gp_pcbs[1];
	} else if (gp_current_process == gp_pcbs[1]) {
		return gp_pcbs[0];
	} else {
		return NULL;
	}
}

int process_switch(PCB *p_pcb_old) {
	PROC_STATE state;
	
	state = gp_current_process->state;

	if (state == NEW) {
		if (gp_current_process != p_pcb_old && p_pcb_old->state != NEW) {
			p_pcb_old->state = READY;
			p_pcb_old->sp = (U32 *) __get_MSP();
		}
		gp_current_process->state = RUN;
		__set_MSP((U32) gp_current_process->sp);
		__rte();  // pop exception stack frame from the stack for a new processes
	} 
	
	/* The following will only execute if the if block above is FALSE */
	if (gp_current_process != p_pcb_old) {
		if (state == READY){ 		
			p_pcb_old->state = READY; 
			p_pcb_old->sp = (U32 *) __get_MSP(); // save the old process's sp
			gp_current_process->state = RUN;
			__set_MSP((U32) gp_current_process->sp); //switch to the new proc's stack    
		} else {
			gp_current_process = p_pcb_old; // revert back to the old proc on error
			return RTX_ERR;
		} 
	}
	return RTX_OK;
}

int k_release_processor() {
	PCB *p_pcb_old = NULL;
	
	p_pcb_old = gp_current_process;
	gp_current_process = scheduler();
	
	if (gp_current_process == NULL) {
		gp_current_process = p_pcb_old; // revert back to the old process
		return RTX_ERR;
	}
	if (p_pcb_old == NULL) {
		p_pcb_old = gp_current_process;
	}
	process_switch(p_pcb_old);
	return RTX_OK;
}

// NULL PROCESS
void set_null_proc() {
	g_proc_table[0].pid = (U32) 0;
	g_proc_table[0].stack_size = 0x100;
	g_proc_table[0].start_pc = &null_process;
	g_proc_table[0].priority = LOWEST + 1;
}

void null_process() {
	while (1) {
		k_release_processor();
	}
}
