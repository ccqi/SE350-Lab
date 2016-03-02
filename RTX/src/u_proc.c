// pUart->IER = IER_THRE | IER_RLS | IER_RBR;
// gp_buffer[0] = 't';
// gp_buffer[1] = 'e';
// gp_buffer[2] = 's';
// gp_buffer[3] = 't';
// gp_buffer[4] = '\0';

#include "u_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_u_procs[1];

void set_u_procs(void) {
  int i;
  for (i = 0; i < 1; i++) {
    g_u_procs[i].priority=LOW;
    g_u_procs[i].stack_size=0x100;
    g_u_procs[i].i_process = 0;
  }
  // g_u_procs[0].pid = PID_PID_SET_PRIO;
  // g_u_procs[0].start_pc = &set_prio_proc;
  g_u_procs[0].pid = PID_CLOCK;
  g_u_procs[0].start_pc = &clock_proc;
}

void clock_proc(void) {
  while (1) {
    release_processor();
  }
}

// void set_prio_proc(void) {
//   while (1) {
//     release_processor();
//   }
// }
