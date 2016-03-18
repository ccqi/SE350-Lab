#include "u_proc.h"

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
	MSG* p; 
	int pid;
	local_queue.first = NULL;
	local_queue.last = NULL;
	while(1) {
		if (message_queue_empty(&local_queue)) {
			p = (MSG*) receive_message(&pid);
		} else {
			p = (MSG*)message_queue_dequeue(&local_queue);
		}
		if (p->type == COUNT_REPORT) {
				if (p->kdata[0] % 20 == 0) {
					send_message(PID_CRT, p);
					hibernate(&local_queue);
				}
		}
		release_memory_block(p);
		release_processor();
	}
}

void hibernate(MSG_QUEUE * queue) {
		MSG *q = (MSG*) request_memory_block();
		q->type = WAKEUP_10;
		
		delayed_send(PID_C, q, 10 * SECOND);
		while(1) {
			int pid;
			MSG* p = (MSG*) receive_message(&pid);
			if (p->type == WAKEUP_10) {
					break;
			} else {
					message_queue_enqueue(queue, p);
			}
			
		}
}

void set_priority_proc(void) {
	int pid;
  int is_valid;
	int proc_id;
	int priority;
	int i;
	int illegal;
	int last_illegal;
	MSG *crt_msg;
  MSG *msg = (MSG*) request_memory_block();
  msg->type = KCD_REG;
  msg->text[0] = '%';
  msg->text[1] = 'C';
  msg->text[2] = '\0';
  send_message(PID_KCD, msg);
	while (1) {
		msg = (MSG*) receive_message(&pid);
		if (msg->type == KCD_CMD) {
			if (msg->text[0] == '%' && msg->text[1] == 'C') {
					is_valid = 1;
					illegal = 0;
					for (i = 2; i < 7 && is_valid; i++) {
						if (i == 2 || i == 5) {
							if (msg->text[i] != ' ') {
								is_valid = 0;
							}
						} else {
							if (!is_int(msg->text[i])) {
								is_valid = 0;
							}
						}
					}
					i--;
					
					if (is_valid) {
						illegal = 1;
						proc_id = get_int(msg->text[3]) * 10 + get_int(msg->text[4]);
						priority = get_int(msg->text[6]);
						if (proc_id < USER_PROC_START || proc_id > USER_PROC_END) {
							is_valid = 0;
						}
						if (priority < HIGH || priority > LOWEST) {
							is_valid = 0;
						}
					} else if (msg->text[i] != '\0') {
						illegal = 1;
					}
					
					if (is_valid && msg->text[7] == '\0') {
						set_process_priority(proc_id, priority);
						crt_msg = (MSG*) request_memory_block();
						crt_msg->type = CRT_DISPLAY;
						crt_msg->text[0] = '\n';
						crt_msg->text[1] = 's';
						crt_msg->text[2] = 'e';
						crt_msg->text[3] = 't';
						crt_msg->text[4] = msg->text[3];
						crt_msg->text[5] = msg->text[4];
						crt_msg->text[6] = ' ';
						crt_msg->text[7] = msg->text[6];
						crt_msg->text[8] = '\0';
						send_message(PID_CRT, crt_msg);
					} else if (illegal) {
						crt_msg = (MSG*) request_memory_block();
						crt_msg->type = CRT_DISPLAY;
						crt_msg->text[0] = '\n';
						crt_msg->text[1] = 'i';
						crt_msg->text[2] = 'l';
						crt_msg->text[3] = 'l';
						crt_msg->text[4] = 'e';
						crt_msg->text[5] = 'g';
						crt_msg->text[6] = 'a';
						crt_msg->text[7] = 'l';
						crt_msg->text[8] = '\0';
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
            h1 = get_int(msg->text[4]);
            h2 = get_int(msg->text[5]);
            m1 = get_int(msg->text[7]);
            m2 = get_int(msg->text[8]);
            s1 = get_int(msg->text[10]);
            s2 = get_int(msg->text[11]);
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
      delayed_send(PID_CLOCK, msg, SECOND);

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
