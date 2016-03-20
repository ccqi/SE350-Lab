#include "u_proc.h"
#include "uart_polling.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_u_procs[5];

void set_u_procs(void) {
  int i;
  for (i = 0; i < 5; i++) {
    g_u_procs[i].priority=HIGH;
    g_u_procs[i].stack_size=0x100;
  }
  // g_u_procs[0].pid = PID_PID_SET_PRIO;
  // g_u_procs[0].start_pc = &set_prio_proc;
  g_u_procs[0].pid = PID_CLOCK;
  g_u_procs[0].start_pc = &clock_proc;
	g_u_procs[1].pid = PID_SET_PRIO;
  g_u_procs[1].start_pc = &set_priority_proc;
	g_u_procs[2].pid = PID_A;
	g_u_procs[2].start_pc = &proc_a;
	g_u_procs[3].pid = PID_B;
	g_u_procs[3].start_pc = &proc_b;
	g_u_procs[4].pid = PID_C;
	g_u_procs[4].start_pc = &proc_c;
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

int get_msg_len(MSG *m) {
  int i;
  while (m->text[i] != '\0')
    i++;
  return i;
}

void msg_put_str(MSG *m, char *s, int l) {
  int i;
  for (i = 0; i < l; i++)
    m->text[i] = s[i];
}

void proc_a(void) {

	int num = 0;
	int pid;
	MSG *msg = (MSG*) request_memory_block();
  msg->type = KCD_REG;
  msg->text[0] = '%';
  msg->text[1] = 'Z';
  msg->text[2] = '\0';
  send_message(PID_KCD, msg);
	while(1) {

		msg = (MSG*) receive_message(&pid);
		if (msg->text[0] == '%' && msg->text[1] == 'Z' && msg->text[2]=='\0') {
      release_memory_block(msg);
			break;
		} else {
			release_memory_block(msg);
		}
	}

	while(1) {
		MSG *p = (MSG*) request_memory_block();
		p->type = COUNT_REPORT;
		p->kdata[0] = num;
		send_message(PID_B, p);
		num++;
		release_processor();
	}
}

void proc_b(void) {
	int pid;
	MSG* msg;
	while (1) {
		msg = (MSG*) receive_message(&pid);
		send_message(PID_C, msg);
	}
}

void proc_c(void) {
	MSG_QUEUE local_queue;
	MSG* p, *q;
	int pid;
	local_queue.first = NULL;
	local_queue.last = NULL;
	q = (MSG*) request_memory_block();
	q->type = WAKEUP_10;
	while(1) {
		if (message_queue_empty(&local_queue)) {
      uart1_put_string("waiting for message from proc_a\r\n");
			p = (MSG*) receive_message(&pid);
		} else {
      uart1_put_string("message queue dequeue\r\n");
			p = (MSG*)message_queue_dequeue(&local_queue);
		}
		if (p->type == COUNT_REPORT) {
				if (p->kdata[0] % 20 == 0) {
					p->type = CRT_DISPLAY;
          msg_put_str(p, "\nproc_c\0", 8);
					send_message(PID_CRT, p);
          uart1_put_string("sending wakeup\r\n");
					delayed_send(PID_C, q, 10);
					while(1) {
						int pid;
						q = (MSG*) receive_message(&pid);
						if (q->type == WAKEUP_10) {
                uart1_put_string("wakeup\r\n");
								break;
						} else {
								message_queue_enqueue(&local_queue, q);
						}
					}
				}
		}
		release_memory_block(p);
		release_processor();
	}
}

void set_priority_proc(void) {
	int pid;
	int proc_id;
	int priority;
  int state;
	int i;
  int l;
  int valid;
	MSG *crt_msg;
  MSG *msg = (MSG*) request_memory_block();
  msg->type = KCD_REG;
  msg->text[0] = '%';
  msg->text[1] = 'C';
  msg->text[2] = '\0';
  send_message(PID_KCD, msg);
	while (1) {
		msg = (MSG*) receive_message(&pid);
		if (msg->type == DEFAULT) {
			if (msg->text[0] == '%' && msg->text[1] == 'C') {
        valid = 0;
        proc_id = 0;
        priority = 0;
        state = 0;
        i = 2;
        while (msg->text[i] != '\0') {
          if (msg->text[i] == ' ') {
            if (state == 0 || state == 2) {
              state++;
            } else {
              state = 0;
              break;
            }
          } else if (is_int(msg->text[i])) {
            if (state == 1 || state == 2) {
              state = 2;
              proc_id *= 10;
              proc_id += get_int(msg->text[i]);
            } else if (state == 3 || state == 4) {
              state = 4;
              priority *= 10;
              priority += get_int(msg->text[i]);
            } else {
              break;
            }
          } else {
            state = 0;
            break;
          }
          i++;
        }

        if (state == 4) {
          valid = 1;
          if (proc_id < SET_PROC_START || proc_id > SET_PROC_END)
            valid = 0;
          if (priority < HIGH || priority > LOWEST)
            valid = 0;
        }

				if (valid) {
					set_process_priority(proc_id, priority);
					crt_msg = (MSG*) request_memory_block();
					crt_msg->type = CRT_DISPLAY;
          msg_put_str(crt_msg, "\nset\0", 5);
					send_message(PID_CRT, crt_msg);
				} else {
					crt_msg = (MSG*) request_memory_block();
					crt_msg->type = CRT_DISPLAY;
          msg_put_str(crt_msg, "\nillegal\0", 9);
					send_message(PID_CRT, crt_msg);
				}
			}
		}
		release_memory_block(msg);
	}
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
  MSG *crt_msg;
  MSG *msg = (MSG*) request_memory_block();
  msg->type = KCD_REG;
  msg->text[0] = '%';
  msg->text[1] = 'W';
  msg->text[2] = '\0';
  send_message(PID_KCD, msg);
  while (1) {
    msg = (MSG*) receive_message(&pid);

    is_start = 0;
    if (msg->type == DEFAULT) {
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
            h1 = get_int(msg->text[4]);
            h2 = get_int(msg->text[5]);
            m1 = get_int(msg->text[7]);
            m2 = get_int(msg->text[8]);
            s1 = get_int(msg->text[10]);
            s2 = get_int(msg->text[11]);

            if (h1 * 10 + h2 > 23) {
              h1 = h2 = 0;
            }
            if (m1 * 10 + m2 > 59) {
              m1 = m2 = 0;
            }
            if (s1 * 10 + s2 > 59) {
              s1 = s2 = 0;
            }
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
      uart1_put_string("clock delay send\r\n");
      delayed_send(PID_CLOCK, msg, 50);

      // Update time
      if (!is_start) {
        if (++s2 > 9) {
          s2 = 0;
          if (++s1 > 5) {
            s1 = 0;
            if (++m2 > 9) {
              m2 = 0;
              if (++m1 > 5) {
                m1 = 0;
                if (++h2 > 9) {
                  h2 = 0;
                  h1++;
                } else if (h1 == 2 && h2 == 4) {
                  h1 = 0;
                  h2 = 0;
                }
              }
            }
          }
        }
      }

      // CRT Display
      crt_msg = (MSG*) request_memory_block();
      crt_msg->type = CRT_DISPLAY;
      crt_msg->text[0] = '\n';
      crt_msg->text[1] = (char) h1 + 48;
      crt_msg->text[2] = (char) h2 + 48;
      crt_msg->text[3] = ':';
      crt_msg->text[4] = (char) m1 + 48;
      crt_msg->text[5] = (char) m2 + 48;
      crt_msg->text[6] = ':';
      crt_msg->text[7] = (char) s1 + 48;
      crt_msg->text[8] = (char) s2 + 48;
      crt_msg->text[9] = '\0';
      send_message(PID_CRT, crt_msg);
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
