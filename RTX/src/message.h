#ifndef _PROCESS_H
#define _PROCESS_H

#include "rtx.h"
#include "queue.h"

int k_send_message(int pid, void *p_msg);
void *k_receive_message(int *p_pid);

#endif
