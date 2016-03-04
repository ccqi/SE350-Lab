#include "s_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

/* initialization table item */
PROC_INIT g_s_procs[2];

// Keyboard input
char *gp_input_buffer;
int input_buffer_index = 0;

// Command registration
MSG *command_identifiers = NULL;

char read_char(int i) {
  return (input_buffer_index - i) >= 0 ? gp_input_buffer[input_buffer_index - i] : gp_input_buffer[input_buffer_index - i + 20];
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
  int i;
  int j;
  int is_command;
  MSG *msg;
  MSG *command_msg;
  MSG *current_msg;
  int pid;
  int lastCommandIndex = 20;
  char in;

  while (1) {
    // Receive message
    msg = (MSG*) receive_message(&pid);

    if (pid == PID_UART_IPROC) { // Input
      // Add input to buffer
      gp_input_buffer[input_buffer_index] = msg->text[0];
      in = msg->text[0];

      if (msg->text[0] == '%') {
        lastCommandIndex = 0;
      } else {
        lastCommandIndex++;
      }

      // Check if command
      if (lastCommandIndex < 20) {
        current_msg = command_identifiers;
        while (current_msg != NULL) {
          // Check if input is command
          is_command = 1;
          i = lastCommandIndex;
          j = 0;
          while (current_msg->text[j] != '\0') {
            if (i < 0 || current_msg->text[j] != read_char(i)) {
              is_command = 0;
              break;
            }
            i--;
            j++;
          }
          if (is_command) {
            command_msg = (MSG*) request_memory_block();
            command_msg->type = KCD_CMD;
            i = lastCommandIndex;
            j = 0;
            while (i >= 0) {
              command_msg->text[j++] = read_char(i--);
            }
            command_msg->text[j] = '\0';
            send_message(current_msg->sPID, command_msg);
            current_msg = NULL;
          } else {
            current_msg = current_msg->next;
          }
        }
      }
      
      // Check if need to loop buffer index
      if (++input_buffer_index >= 20) {
        input_buffer_index = 0;
      }

      // Send last input to CRT process
      msg->type = CRT_DISPLAY;
      msg->text[0] = in;
      msg->text[1] = '\0';
      send_message(PID_CRT, msg);
    } else if (msg->type == KCD_REG) { // KCD, add msg to command_identifiers linked list
      msg->next = NULL;
      if (command_identifiers == NULL) {
        command_identifiers = msg;
      } else {
        current_msg = command_identifiers;
        while (current_msg->next != NULL) {
          current_msg = current_msg->next;
        }
        current_msg->next = msg;
      }
    } else {
      release_memory_block(msg);
    }
    release_processor();
  }
}

void crt_proc(void) {
  int pid;
  MSG *msg;
  int i;
  LPC_UART_TypeDef *pUart;
  pUart = (LPC_UART_TypeDef *)LPC_UART0;
  while (1) {
    msg = (MSG*) receive_message(&pid);
    if (msg->type == CRT_DISPLAY) {
      i = 0;
      while (msg->text[i] != '\0') {
        gp_buffer[i] = msg->text[i];
        i++;
      }
      if (i > 0) {
        gp_buffer[i] = '\0';
        pUart->IER = IER_THRE | IER_RLS | IER_RBR;
      }
    }
    release_memory_block(msg);
    release_processor();
  }
}
