#ifndef S_PROC_H
#define S_PROC_H

#include <LPC17xx.h>
#include "uart.h"
#include "rtx.h"

extern uint8_t *gp_buffer;

void set_s_procs(void);
void kcd_proc(void);
void crt_proc(void);

#endif /* S_PROC_H_ */
