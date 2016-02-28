#include "s_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_s_procs[2];

void set_s_procs(void) {
  int i;
  for (i = 0; i < 2; i++) {
    g_s_procs[i].priority=LOWEST;
    g_s_procs[i].stack_size=0x100;
    g_s_procs[i].i_process = 0;
  }
  g_s_procs[0].pid = PID_KCD;
  g_s_procs[0].start_pc = &kcd_proc;
  g_s_procs[1].pid = PID_CRT;
  g_s_procs[1].start_pc = &crt_proc;
}

void kcd_proc(void) {
  while (1) {
    __disable_irq();
    __enable_irq();
    i_release_processor();
  }
}

void crt_proc(void) {
  while (1) {
    __disable_irq();
    __enable_irq();
    i_release_processor();
  }
}
