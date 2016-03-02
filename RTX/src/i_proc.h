#ifndef I_PROC_H
#define I_PROC_H

#include <LPC17xx.h>
#include "uart.h"
#include "uart_polling.h"
#include "rtx.h"
#include "timer.h"
#include "message_queue.h"
#include "process_queue.h"

extern volatile uint32_t g_timer_count;
extern MSG_QUEUE *gp_timeout_queue;

// UART
extern uint8_t g_char_in;

// PCB Queue
extern PROC_QUEUE **gp_pcb_queue;

extern void i_release_processor(void);
extern int i_send_message(void *p_msg);

extern void *i_request_memory_block(void);

void set_i_procs(void);
void timer_proc(void);
void uart_proc(void);

#endif /* I_PROC_H_ */
