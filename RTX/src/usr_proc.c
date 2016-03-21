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

int test_1 = 0;
int test_2 = 0;
int test_3 = 0;
int test_4 = 0;
int test_5 = 0;
int test_6 = 0;

int test_1_ok = 0;

int test_2_ok_1 = 0;
int test_2_ok_2 = 0;
int test_2_ok_3 = 0;

int test_5_ok_1 = 0;
int test_5_ok_2 = 0;

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
	if (test_1) {
		print_test_OK(1);
	} else {
		print_test_FAIL(1);
	}
	if (test_2) {
		print_test_OK(2);
	} else {
		print_test_FAIL(2);
	}
	if (test_3) {
		print_test_OK(3);
	} else {
		print_test_FAIL(3);
	}
	if (test_4) {
		print_test_OK(4);
	} else {
		print_test_FAIL(4);
	}
	if (test_5) {
		print_test_OK(5);
	} else {
		print_test_FAIL(5);
	}
	if (test_6) {
		print_test_OK(6);
	} else {
		print_test_FAIL(6);
	}

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

	g_test_procs[0].priority = MEDIUM;
}

// TEST DELAYED SEND
void proc1(void) {
	int sender_id;
	MSG *msg;

	print_test_start();

	msg = (MSG*) request_memory_block();
	msg->type = DEFAULT;
	msg->text[0] = 'a';
	msg->text[1] = 'b';
	msg->text[2] = 'c';
	msg->text[3] = '\0';
	delayed_send(1, msg, 5);
	set_process_priority(2, LOW);
	set_process_priority(1, HIGH);
	msg = (MSG*) receive_message(&sender_id); // goto proc2 until delay finished
	if (test_1_ok == 1
			&& sender_id == 1
			&& msg->type == DEFAULT 
			&& msg->text[0] == 'a'
			&& msg->text[1] == 'b'
			&& msg->text[2] == 'c')
		test_1 = 1;
	else
		test_1 = 0;
	release_memory_block(msg);
	set_process_priority(1, LOWEST);
	
	while (1) {
		release_processor();
	}
}

// Test message sending, message sending preemption
void proc2(void)
{
	int i;
	MSG *msg = (MSG*) request_memory_block();
	test_1_ok = 1;
	for (i = 0; i < 1000000; i++) {
		
	}
	set_process_priority(2, LOW);
	set_process_priority(3, MEDIUM); // go to proc3

	if (test_2_ok_1) {
		test_2_ok_2 = 1;
	}

	msg->type = DEFAULT;
	msg->text[0] = '1';
	msg->text[1] = '\0';

	send_message(3, msg);

	if (test_2_ok_1 && test_2_ok_2 && test_2_ok_3) {
		test_2 = 1;
	} else {
		test_2 = 0;
	}

	while (1) {
		release_processor();
	}
}

// Wait for message from proc2, should come back to this method after message sent
void proc3(void) {
	int sender_id;
	MSG *msg;
	test_2_ok_1 = 1;

	msg = (MSG*) receive_message(&sender_id);
	if (test_2_ok_2 && sender_id == 2 && msg->text[0] == '1' && msg->text[1] == '\0') {
		test_2_ok_3 = 1;
	}
	release_memory_block(msg);
	set_process_priority(2, MEDIUM);
	release_processor(); // go to process 2

	set_process_priority(4, MEDIUM);
	set_process_priority(3, LOWEST);
	while (1) {
		release_processor();
	}
}

// Test request/release memory
void proc4(void) {
	int i;
	int ret;
	void *block;
	test_3 = 1;
	for (i = 0; i < 10; i++) {
		block = request_memory_block();
		if (block == NULL) {
			test_3 = 0;
		}
		ret = release_memory_block(block);
		if (ret == RTX_ERR) {
			test_3 = 0;
		}
	}

	set_process_priority(5, MEDIUM);
	set_process_priority(4, LOWEST);

	while (1) {
		release_processor();
	}
}

// Test get/set_process_priority
// Test preemption with same priorities
void proc5(void)
{	
	int i;

	test_4 = 1;
	set_process_priority(5, HIGH);
	if (get_process_priority(5) != HIGH) {
		test_4 = 0;
	}
	set_process_priority(1, MEDIUM);
	set_process_priority(2, LOW);
	set_process_priority(3, LOWEST);
	set_process_priority(4, LOW);
	if (get_process_priority(1) != MEDIUM) {
		test_4 = 0;
	}
	if (get_process_priority(2) != LOW) {
		test_4 = 0;
	}
	if (get_process_priority(3) != LOWEST) {
		test_4 = 0;
	}
	if (get_process_priority(2) != LOW) {
		test_4 = 0;
	}

	for (i = 1; i < 5; i++) {
		set_process_priority(i, LOWEST);
	}

	set_process_priority(5, HIGH);
	set_process_priority(6, HIGH);
	if (test_5_ok_1) {
		test_5_ok_2 = 1;
	}
	release_processor();

	while (1) {
		release_processor();
	}
}

// Test return values.
void proc6(void)
{
	int r1;
	int r2;
	int r3;
	int r4;

	test_5_ok_1 = 1;
	release_processor();
	if (test_5_ok_2) {
		test_5 = 1;
	}

	r1 = set_process_priority(1, HIGH - 1);
	r2 = release_memory_block(NULL);
	r3 = get_process_priority(-1);
	r4 = set_process_priority(5, LOWEST);

	if (r1 == -1 && r2 == -1 && r3 == -1 && r4 == 0) {
		test_6 = 1;
	}
	print_test_results();
	set_process_priority(6, MEDIUM);

	while (1) {
		release_processor();
	}
}
