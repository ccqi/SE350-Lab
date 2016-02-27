#ifndef I_PROC_H
#define I_PROC_H

#include "uart_polling.h"
#include "rtx.h"
#include "timer.h"

extern volatile uint32_t g_timer_count;

void set_i_procs(void);
void timer_proc(void);
void uart_proc(void);

#endif /* I_PROC_H_ */
