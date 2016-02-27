#include "rtx.h"
#include "uart_polling.h"
#include "usr_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

void proc0(void);

/* initialization table item */
PROC_INIT g_test_procs[NUM_TEST_PROCS];

// TESTING VARIABLES
int test_ok = 0;
int test_fail = 0;
int test_num = 6;

int test_2_ok = 0;

int test_3_ok_1 = 0;
int test_3_ok_2 = 0;
int test_3_ok_3 = 0;
int test_3_ok_4 = 0;

int test_6_ok = 0;

void print_test_start() {
	uart1_put_string("G001_test: START\r\n");
	uart1_put_string("G001_test: total ");
	uart1_put_char((unsigned char)(test_num + 48));
	uart1_put_string(" tests\r\n");
}

void print_test_OK(int n) {
	uart1_put_string("G001_test: test ");
	uart1_put_char((unsigned char)(n + 48));
	uart1_put_string(" OK\r\n");
	test_ok++;
}

void print_test_FAIL(int n) {
	uart1_put_string("G001_test: test ");
	uart1_put_char((unsigned char)(n + 48));
	uart1_put_string(" FAIL\r\n");
	test_fail++;
}

void print_test_results() {
	uart1_put_string("G001_test: ");
	uart1_put_char((unsigned char)(test_ok + 48));
	uart1_put_string("/");
	uart1_put_char((unsigned char)(test_num + 48));
	uart1_put_string(" tests OK\r\n");

	uart1_put_string("G001_test: ");
	uart1_put_char((unsigned char)(test_fail + 48));
	uart1_put_string("/");
	uart1_put_char((unsigned char)(test_num + 48));
	uart1_put_string(" tests FAIL\r\n");
	uart1_put_string("G001_test: END\r\n");
}

void set_test_procs() {
	int i;
	for(i = 0; i < NUM_TEST_PROCS; i++) {
		g_test_procs[i].pid=(U32)(i+1);
		g_test_procs[i].priority=LOWEST;
		g_test_procs[i].stack_size=0x100;
		g_test_procs[i].i_process = 0;
	}
  
	// g_test_procs[0].start_pc = &proc1;
	g_test_procs[0].start_pc = &proc0;
	g_test_procs[1].start_pc = &proc2;
	g_test_procs[2].start_pc = &proc3;
	g_test_procs[3].start_pc = &proc4;
	g_test_procs[4].start_pc = &proc5;
	g_test_procs[5].start_pc = &proc6;

	g_test_procs[0].priority = HIGH;
}

void proc0(void) {
	while (1) {

	}
}

// Request 4 memory blocks, then release all 4.
// Lowers proc1 to LOWEST priority to run proc2.
void proc1(void)
{
	int i;
	int ret;
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;
	print_test_start();

	// Reduce memory blocks to 4
	for (i = 0; i < NUM_MEMORY_BLOCKS - 4; i++) {
		block1 = request_memory_block();
	}

	while (1) {
		block1 = request_memory_block();
		block2 = request_memory_block();
		block3 = request_memory_block();
		block4 = request_memory_block();
		for (i = 0; i < 10000; i++); // Delay
		ret = release_memory_block(block1);
		ret = ret || release_memory_block(block2);
		ret = ret || release_memory_block(block3);
		ret = ret || release_memory_block(block4);

		if (ret == 0) {
			print_test_OK(1);
		} else {
			print_test_FAIL(1);
		}

		set_process_priority(1, LOWEST); // Go to proc2
	}
}

// Request 4 blocks.
// set to medium priority.
// set proc3 priority to high.
// proc2 will be preempted, and proc3 will run.
void proc2(void)
{
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;

	// todo fix stuff
	while (1) {

	}

	while (1) {
		block1 = request_memory_block();
		block2 = request_memory_block();
		block3 = request_memory_block();
		block4 = request_memory_block();
		set_process_priority(2, MEDIUM);
		set_process_priority(3, HIGH);

		// proc3 not run after priority set
		if (!test_2_ok) {
			print_test_FAIL(2);
			test_2_ok = -1;
		}

		release_memory_block(block1);
		if (!test_3_ok_1) {
			test_3_ok_1 = -1;
		}
		release_memory_block(block2);
		if (!test_3_ok_2) {
			test_3_ok_2 = -1;
		}
		release_memory_block(block3);
		if (!test_3_ok_3) {
			test_3_ok_3 = -1;
		}
		release_memory_block(block4);
		if (!test_3_ok_4) {
			test_3_ok_4 = -1;
		}
	}
}

// Tests memory blocking with preemption.
// First request should block process and run proc2
// Should alternate running proc2 and proc3 until all blocks freed from proc2 and given to proc 3
void proc3(void) {
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;

	// proc2 successfully preempted
	if (test_2_ok != -1) {
		print_test_OK(2);
		test_2_ok = 1;
	}

	while (1) {
		block1 = request_memory_block();
		if (!test_3_ok_1) {
			test_3_ok_1 = 1;
		}
		block2 = request_memory_block();
		if (!test_3_ok_2) {
			test_3_ok_2 = 1;
		}
		block3 = request_memory_block();
		if (!test_3_ok_3) {
			test_3_ok_3 = 1;
		}
		block4 = request_memory_block();
		if (!test_3_ok_4) {
			test_3_ok_4 = 1;
		}

		if (test_3_ok_1 == 1 && test_3_ok_2 == 1 && test_3_ok_3 == 1 && test_3_ok_4 == 1) {
			print_test_OK(3);
		} else {
			print_test_FAIL(3);
		}

		release_memory_block(block1);
		release_memory_block(block2);
		release_memory_block(block3);
		release_memory_block(block4);

		// Go to proc4
		set_process_priority(2, LOWEST);
		set_process_priority(3, LOWEST);
	}
}

// Test for priorities, should stay in proc4 for the test
void proc4(void) {
	int p1;
	int p2;
	int p3;
	int p4;
	int p5;
	while (1) {
		p1 = get_process_priority(1);
		p2 = get_process_priority(2);
		p3 = get_process_priority(3);
		set_process_priority(4, MEDIUM);
		p4 = get_process_priority(4);
		set_process_priority(5, LOWEST);
		p5 = get_process_priority(5);
		if (p1 == p2 && p2 == p3 && p3 == LOWEST && p4 == MEDIUM && p5 == LOWEST) {
			print_test_OK(4);
		} else {
			print_test_FAIL(4);
		}
		// Go to proc5
		set_process_priority(5, HIGH);
	}
}

// Test ok and error return values
void proc5(void)
{
	int ret1;
	int ret2;
	int ret3;
	int ret4;
	void *block = NULL;
	while (1) {
		// Change null priority
		ret1 = set_process_priority(0, LOWEST);
		// Change invalid process priority
		ret2 = set_process_priority(-1, HIGH);
		// Change process to invalid priority
		ret3 = set_process_priority(1, -2);
		// Change process to null process priority
		ret4 = set_process_priority(2, 4);
		if (ret1 && ret2 && ret3 && ret4) {
			ret1 = set_process_priority(1, LOWEST);
			ret2 = release_memory_block(0);
			if (!ret1 && ret2) {
				print_test_OK(5);
			} else {
				print_test_FAIL(5);
			}
		} else {
			print_test_FAIL(5);
		}

		// Go to proc6
		set_process_priority(6, HIGH);
		// Since same priority as proc5, proc5 should continue to run

		block = request_memory_block();
		set_process_priority(5, MEDIUM);
		test_6_ok = 1;
		release_memory_block(block);
		// This should never run
		test_6_ok = 0;
	}
}

// Test memory blocking.
void proc6(void)
{
	int i;
	while (1) {
		for (i = 0; i < 4; i++) {
			request_memory_block();
		}
		if (test_6_ok) {
			print_test_OK(6);
		} else {
			print_test_FAIL(6);
		}
		print_test_results();

		while (1) {
			release_processor();
		}
	}
}
