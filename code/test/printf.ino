static int uart_putchar (char c, FILE *stream)
{
    Serial.write(c) ;
    return 0 ;
}

void p(char *fmt, ... ){
        char tmp[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(tmp, 128, fmt, args);
        va_end (args);
}

void printfSetup(){
#include <stdarg.h>
#include <stdio.h>

	// we need fundamental FILE definitions and printf declarations

	// create a FILE structure to reference our UART output function

	static FILE uartout = {0} ;
	fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
}
