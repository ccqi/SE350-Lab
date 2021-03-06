#ifndef _PROCESS_H
#define _PROCESS_H

#include "rtx.h"
#include "message_queue.h"
#include "process_queue.h"

extern PCB *gp_current_process;
extern PROC_QUEUE **gp_pcb_queue;

extern int k_release_processor(void);

int k_send_message(int pid, void *p_msg);
void *k_receive_message(int *p_pid);
int k_delayed_send(int process_id, void *message_envelope, int delay);

int i_send_message(void *p_msg);

#endif
