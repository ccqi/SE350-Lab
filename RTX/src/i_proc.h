#ifndef I_PROC_H
#define I_PROC_H

#include <LPC17xx.h>
#include "uart.h"
#include "uart_polling.h"
#include "rtx.h"
#include "timer.h"
#include "message_queue.h"

extern volatile uint32_t g_timer_count;
extern MSG_QUEUE *gp_timeout_queue;

extern void i_release_processor(void);
extern int i_send_message(void *p_msg);

void set_i_procs(void);
void timer_proc(void);
void uart_proc(void);

#endif /* I_PROC_H_ */
