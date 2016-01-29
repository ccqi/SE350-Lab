#include <LPC17xx.h>
#include "uart_polling.h"
#include "../src/memory.h"
#include "../src/process.h"

int main() {
	SystemInit();
	uart0_init();
	memory_init();
	uart0_put_string("Hello World!\n\r");
	return 0;
}
