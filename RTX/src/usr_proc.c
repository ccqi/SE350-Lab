/**
 * @file:   usr_proc.c
 * @brief:  Two user processes: proc1 and proc2
 * @author: Yiqing Huang
 * @date:   2014/02/28
 * NOTE: Each process is in an infinite loop. Processes never terminate.
 */

#include "rtx.h"
#include "uart_polling.h"
#include "usr_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_test_procs[NUM_TEST_PROCS];

void set_test_procs() {
	int i;
	for( i = 0; i < NUM_TEST_PROCS; i++ ) {
		g_test_procs[i].pid=(U32)(i+1);
		g_test_procs[i].priority=LOWEST;
		g_test_procs[i].stack_size=0x100;
	}
  
	g_test_procs[0].start_pc = &proc1;
	g_test_procs[1].start_pc = &proc2;
	g_test_procs[2].start_pc = &proc3;
	g_test_procs[2].priority = LOW;
	// g_test_procs[3].priority = MEDIUM;
}


/**
 * @brief: a process that prints 5x6 uppercase letters
 *         and then yields the cpu.
 */
void proc1(void)
{
	int i = 0;
	int ret_val = 10;
	int x = 0;

	while ( 1) {
		if ( i != 0 && i%5 == 0 ) {
			uart1_put_string("\n\r");
			
			if ( i%30 == 0 ) {
				ret_val = release_processor();
#ifdef DEBUG_0
				printf("proc1: ret_val=%d\n", ret_val);
			
#endif /* DEBUG_0 */
			}
			for ( x = 0; x < 500000; x++); // some artifical delay
		}
		uart1_put_char('A' + i%26);
		i++;
		
	}
}

/**
 * @brief: a process that prints 5x6 numbers
 *         and then yields the cpu.
 */
void proc2(void)
{
	int i = 0;
	int ret_val = 20;
	int x = 0;
	while ( 1) {
		if ( i != 0 && i%5 == 0 ) {
			uart1_put_string("\n\r");
			
			if ( i%30 == 0 ) {
				ret_val = release_processor();
				#ifdef DEBUG_0
				printf("proc2: ret_val=%d\n", ret_val);
				#endif /* DEBUG_0 */
			}
			for ( x = 0; x < 500000; x++); // some artifical delay
		}
		uart1_put_char('0' + i%10);
		i++;
		
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

		release_processor();
	}
}

void proc4(void) {
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;
	while (1) {
		uart0_put_string("Hello proc4\n");
		block1 = request_memory_block();
		block2 = request_memory_block();
		block3 = request_memory_block();
		block4 = request_memory_block();
		uart0_put_string("proc4: Request 4 memory blocks.\n");
		release_memory_block(block1);
		release_memory_block(block2);
		release_memory_block(block3);
		release_memory_block(block4);
		uart0_put_string("proc4: Release 4 memory blocks.\n");
		uart0_put_string("proc4: Set proc3 priority to HIGH.\n");
		// set_process_priority(3, HIGH);

		#ifdef DEBUG_0
		// printf("proc4: proc3 priority: %x \n", get_process_priority(3));
		#endif

		release_processor();
	}
}
