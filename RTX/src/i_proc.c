#include "i_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

// UART
extern uint8_t g_buffer[];
extern uint8_t *gp_buffer;
extern uint8_t g_send_char;
extern uint8_t g_char_in;
extern uint8_t g_char_out;
extern uint32_t g_switch_flag;

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
	g_i_procs[0].pid = PID_TIMER_IPROC;
	g_i_procs[0].start_pc = &timer_proc;
	g_i_procs[1].pid = PID_UART_IPROC;
	g_i_procs[1].start_pc = &uart_proc;
}

void timer_proc(void) {
	MSG *message;
	MSG *prev_message;

	while (1) {
		__disable_irq();
		g_timer_count++;

		if (g_timer_count >= SECOND) {
			sec++;
			g_timer_count = 0;
			#ifdef DEBUG_0
				printf("timer = 0x%x \n", sec);
			#endif
		}

		message = gp_timeout_queue->first;
		prev_message = NULL;
		while (message != NULL) {
			message->expiry--;
			if (message->expiry <= 0) {
				i_send_message(message);

				if (prev_message != NULL) {
					prev_message->next = message->next;
					message->next = NULL;
					message = prev_message->next;
				} else {
					gp_timeout_queue->first = message->next;
					message->next = NULL;
					message = gp_timeout_queue->first;
				}

				if (gp_timeout_queue->first == NULL) {
					gp_timeout_queue->last = NULL;
				}
			} else {
				prev_message = message;	
				message = (MSG*) message->next;
			}
		}
		__enable_irq();
		release_processor();
	}
}

void uart_proc(void) {
	uint8_t IIR_IntId;	    // Interrupt ID from IIR
	LPC_UART_TypeDef *pUart;
	while (1) {	
		__disable_irq();
		pUart = (LPC_UART_TypeDef *)LPC_UART0;

		#ifdef DEBUG_0
		uart1_put_string("Entering c_UART0_IRQHandler\n\r");
		#endif // DEBUG_0

		if (g_send_char) {
			#ifdef DEBUG_0
			uart1_put_string("Reading a char = ");
			uart1_put_char(g_char_in);
			uart1_put_string("\n\r");
			#endif // DEBUG_0
		}
		__enable_irq();
		release_processor();
	}
}
