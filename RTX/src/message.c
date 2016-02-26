#include "message.h"

int k_send_message(int pid, void *p_msg) {
	PCB* pcb = process_find(gp_pcb_queue, pid);
	MSG *msg = (MSG*) p_msg;

	if (pcb == NULL || msg == NULL) {
		return RTX_ERR;
	}
	__disable_irq();
	msg->sPID = (int) gp_current_process->id;
	msg->rPID = pid;

	message_queue_enqueue(pcb->message_queue, p_msg);

	if (pcb->state == BLOCKED_ON_RECEIVE) {
		pcb->state = READY;
		if (pcb->priority > gp_current_process->priority) {
			k_release_processor();
		}
	}
	__enable_irq();

	return RTX_OK;
}

void *k_receive_message(int *p_pid) {
	MSG_QUEUE *q;
	__disable_irq();
	q = gp_current_process->message_queue;
	while (message_queue_empty(q)) {
		gp_current_process->state = BLOCKED_ON_RECEIVE;
		k_release_processor();
	}
	__enable_irq();
	return message_queue_dequeue(q);
}
