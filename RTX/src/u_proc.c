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
    g_u_procs[i].priority=HIGH;
    g_u_procs[i].stack_size=0x100;
    g_u_procs[i].i_process = 0;
  }
  // g_u_procs[0].pid = PID_PID_SET_PRIO;
  // g_u_procs[0].start_pc = &set_prio_proc;
  g_u_procs[0].pid = PID_CLOCK;
  g_u_procs[0].start_pc = &clock_proc;
}

int is_int(char c) {
  int i = (int) c - 48;
  if (i >= 0 && i <= 9)
    return 1;
  else
    return 0;
}

int get_int(char c) {
  return (int) c - 48;
}

void clock_proc(void) {
  int h1 = 0;
  int h2 = 0;
  int m1 = 0;
  int m2 = 0;
  int s1 = 0;
  int s2 = 0;
  int is_running = 0;
  int is_start = 0;
  int i;
  int pid;
  int is_valid;
  MSG *msg = (MSG*) request_memory_block();
  msg->type = KCD_REG;
  msg->text[0] = '%';
  msg->text[1] = 'W';
  msg->text[2] = '\0';
  send_message(PID_KCD, msg);
  while (1) {
    msg = (MSG*) receive_message(&pid);

    is_start = 0;
    if (msg->type == KCD_CMD) {
      if (msg->text[0] == '%' && msg->text[1] == 'W') {
        if (msg->text[2] == 'R' && msg->text[3] == '\0') {
          h1 = h2 = m1 = m2 = s1 = s2 = 0;
          is_running = 1;
          is_start = 1;
        } else if (msg->text[2] == 'S' && msg->text[3] == ' ') {
          // hh:mm:ss
          is_valid = 1;
          for (i = 4; i < 12 && is_valid; i++) {
            if (i == 6 || i == 9) {
              if (msg->text[i] != ':') {
                is_valid = 0;
              }
            } else {
              if (!is_int(msg->text[i])) {
                is_valid = 0;
              }
            }
          }
          if (is_valid && msg->text[12] == '\0') {
            h1 = get_int(4);
            h1 = get_int(5);
            m1 = get_int(7);
            m1 = get_int(8);
            s1 = get_int(10);
            s1 = get_int(11);
            is_running = 1;
            is_start = 1;
          }
        } else if (msg->text[2] == 'T' && msg->text[3] == '\0') {
          is_running = 0;
        }
      }
    }
    if (is_start || (msg->type == DEFAULT && pid == PID_CLOCK && is_running)) {
      msg->type = DEFAULT;
      msg->text[0] = '\0';
      delayed_send(PID_CLOCK, msg, 50);
    } else {
      release_memory_block(msg);
    }
    release_processor();
  }
}

// void set_prio_proc(void) {
//   while (1) {
//     release_processor();
//   }
// }
