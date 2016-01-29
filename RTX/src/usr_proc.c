#include "rtx.h"
#include "uart_polling.h"
#include "usr_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_test_procs[NUM_TEST_PROCS];

// TESTING VARIABLES
int test_ok = 0;
int test_fail = 0;
int test_num = 6;

void print_test_results() {
	uart1_put_string("\r\n");
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
	}
  
	g_test_procs[0].start_pc = &proc1;
	g_test_procs[1].start_pc = &proc2;
	g_test_procs[2].start_pc = &proc3;
	g_test_procs[3].start_pc = &proc4;
	g_test_procs[4].start_pc = &proc5;
	g_test_procs[5].start_pc = &proc6;

	g_test_procs[0].priority = HIGH;
	print_test_results();
}


void proc1(void)
{
	uart1_put_string("\n\r");
	while (1) {
		release_processor();
	}
}

void proc2(void)
{
	uart1_put_string("\n\r");
	while (1) {
		release_processor();
	}
}

void proc3(void) {
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;
	while (1) {
		uart0_put_string("Hello proc3\n");
		block1 = request_memory_block();
		block2 = request_memory_block();
		block3 = request_memory_block();
		block4 = request_memory_block();
		uart0_put_string("proc3: Request 4 memory blocks.\n");
		release_memory_block(block1);
		release_memory_block(block2);
		release_memory_block(block3);
		release_memory_block(block4);
		uart0_put_string("proc3: Release 4 memory blocks.\n");
		uart0_put_string("proc3: Lower priority to LOWEST.\n");
		set_process_priority(3, LOWEST);

		#ifdef DEBUG_0
		printf("proc3: Priority: %x \n", get_process_priority(3));
		#endif

		set_process_priority(1, HIGH);

		release_processor();
	}
}

void proc4(void) {
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;
	void *block5 = NULL;
	while (1) {
		uart0_put_string("Hello proc4\n");
		block1 = request_memory_block();
		block2 = request_memory_block();
		block3 = request_memory_block();
		block4 = request_memory_block();
		block5 = request_memory_block();
		uart0_put_string("proc4: Request 4 memory blocks.\n");
		release_memory_block(block1);
		release_memory_block(block2);
		release_memory_block(block3);
		release_memory_block(block4);
		uart0_put_string("proc4: Release 4 memory blocks.\n");
		uart0_put_string("proc4: Set proc3 priority to HIGH.\n");
		set_process_priority(3, HIGH);
		uart0_put_string("proc4: Lower priority to LOW.\n");
		set_process_priority(4, LOW);

		#ifdef DEBUG_0
		printf("proc4: Priority: %x \n", get_process_priority(4));
		printf("proc4: proc3 priority: %x \n", get_process_priority(3));
		#endif

		release_processor();
	}
}

void proc5(void)
{
	uart1_put_string("\n\r");
	while (1) {
		release_processor();
	}
}

void proc6(void)
{
	uart1_put_string("\n\r");
	while (1) {
		release_processor();
	}
}
