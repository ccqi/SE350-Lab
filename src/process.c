#include "rtx.h"

PCB **gp_pcbs;
PCB *gp_current_process = 0;

PROC_INIT g_proc_table[NUM_TEST_PROCS];
extern PROC_INIT g_test_procs[NUM_TEST_PROCS];

void process_init() {

}

int set_process_priority(int process_id, int priority) {
	return 0;
}

int get_process_priority(int process_id) {

	return -1;
}

int release_processor() {
	return 0;
}
