/**
 * @file:   main_svc.c
 * @brief:  main routine to start up the RTX and processes
 * @author: Yiqing Huang
 * @date:   2014/01/17
 * NOTE: standard C library is not allowed in the final kernel code.
 *       A tiny printf function for embedded application development
 *       taken from http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
 *       is configured to use UART0 to output when DEBUG_0 is defined.
 *       Check target option->C/C++ to see the DEBUG_0 definition.
 *       Note that init_printf(NULL, putc) must be called to initialize 
 *       the printf function.
 */

#include <LPC17xx.h>
#include <system_LPC17xx.h>
#include "rtx.h"
#ifdef DEBUG_0
#include "uart_polling.h"
#include "printf.h"
#endif /* DEBUG_0 */

extern volatile unsigned int g_timer_count;

int main() 
{	
	/* CMSIS system initialization */
	SystemInit(); 
#ifdef DEBUG_0
	init_printf(NULL, putc);
#endif /* DEBUG_0 */
	timer_init(0);
	/* start the RTX and built-in processes */
	rtx_init();  
  
	/* We should never reach here!!! */
	return RTX_ERR;  
}

/* Timer
	volatile uint8_t sec = 0;

	SystemInit();
	__disable_irq();
	timer_init(0); // initialize timer 0 
 
	  uart polling is used in this example only to help
	  demonstrate timer interrupt programming.
	  In your final project, polling is NOT ALLOWED in i-processes.
	  Refer to the UART_irq example for interrupt driven UART.

	uart0_init();

	__enable_irq();

	while (1) {
		// g_timer_count gets updated every 1ms
		if (g_timer_count == 1000) { 
			uart0_put_char('0'+ sec);
			sec = (++sec)%10;
			g_timer_count = 0; // reset the counter
		}     
	}
*/
