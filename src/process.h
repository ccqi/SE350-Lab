#ifndef _PROCESS_H
#define _PROCESS_H

#include "rtx.h"

extern PROC_INIT g_test_procs[NUM_TEST_PROCS];

void process_init();
int set_process_priority(int process_id, int priority);
int get_process_priority(int process_id);
int release_processor();

#endif