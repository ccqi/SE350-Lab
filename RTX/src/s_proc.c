#include "s_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_s_procs[2];

// Keyboard input
char *gp_input_buffer;
int input_buffer_index = 0;

char read_char(int i) {
  return (input_buffer_index - i) >= 0 ? gp_input_buffer[input_buffer_index - i] : gp_input_buffer[input_buffer_index - i + 10];
}

void set_s_procs(void) {
  int i;
  for (i = 0; i < 2; i++) {
    g_s_procs[i].priority=HIGH;
    g_s_procs[i].stack_size=0x100;
    g_s_procs[i].i_process = 0;
  }
  g_s_procs[0].pid = PID_KCD;
  g_s_procs[0].start_pc = &kcd_proc;
  g_s_procs[1].pid = PID_CRT;
  g_s_procs[1].start_pc = &crt_proc;
}

void kcd_proc(void) {
  char in;
  MSG *msg;
  int pid;

  while (1) {
    // Receive message
    msg = (MSG*) receive_message(&pid);

    if (pid == PID_UART_IPROC) { // Input
      // Add input to buffer
      gp_input_buffer[input_buffer_index] = msg->text[0];

      // Check if command
      in = read_char(0);

      // Check if need to loop buffer index
      if (++input_buffer_index > 9) {
        input_buffer_index = 0;
      }
    } else { // KCD
      in = msg->text[0];
    }
    release_processor();
  }
}

void crt_proc(void) {
  while (1) {
    release_processor();
  }
}
