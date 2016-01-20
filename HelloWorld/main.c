#include <LPC17xx.h>
#include "uart_polling.h"
int main() {
	uint32_t* a = (uint32_t*)(0x10004000);
	uint32_t b = *a;
	uint32_t* c = &b;
	*a = 0x11111111;
	b = *a;
	SystemInit();
	uart0_init();
	uart0_put_string("Hello World!\n\r");
	return 0;
}
