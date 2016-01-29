#include <LPC17xx.h>
#include <system_LPC17xx.h>
#include "uart_polling.h"
#include "../src/memory.h"
#include "../src/process.h"

#ifdef DEBUG_0
#include "../src/printf.h"
#endif

int main() {
	__disable_irq();
	SystemInit(); 
	#ifdef DEBUG_0
	init_printf(NULL, putc);
	printf("DEBUG");
	#endif
	uart0_init();
	memory_init();
	process_init();
	__enable_irq();
	uart0_put_string("Hello World!\n\r");
	
	release_processor();
	return 0;
}
