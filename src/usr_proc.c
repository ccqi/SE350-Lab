#include "rtx.h"

extern int release_processor();

PROC_INIT g_test_procs[NUM_TEST_PROCS];

void proc1();
void proc2();

void set_test_procs() {
	int i;
	for (i = 0; i < NUM_TEST_PROCS; i++) {
		g_test_procs[i].pid = (U32)(i + 1);
		g_test_procs[i].priority = LOWEST;
		g_test_procs[i].stack_size = 0x100;
	}

	g_test_procs[0].start_pc = &proc1;
	g_test_procs[1].start_pc = &proc2;
}

void proc1() {
	while (1) {
		release_processor();
	}
}

void proc2() {
	while (1) {
		release_processor();
	}
}
