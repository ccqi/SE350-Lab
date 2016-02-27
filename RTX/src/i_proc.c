#include "i_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_i_procs[2];

volatile uint8_t sec = 0;

void set_i_procs(void) {
	int i;
	for (i = 0; i < 2; i++) {
		g_i_procs[i].priority=LOWEST;
		g_i_procs[i].stack_size=0x100;
		g_i_procs[i].i_process = 1;
	}
	g_i_procs[0].pid = 14;
	g_i_procs[0].start_pc = &timer_proc;
	g_i_procs[1].pid = 15;
	g_i_procs[1].start_pc = &uart_proc;
}

void timer_proc(void) {
	while (1) {
		__disable_irq();
		if (g_timer_count >= SECOND) {
			sec++;
			g_timer_count = 0;
			#ifdef DEBUG_0
				printf("timer = 0x%x \n", sec);
			#endif
		}
		__enable_irq();
		release_processor();
	}
}

void uart_proc(void) {

}
