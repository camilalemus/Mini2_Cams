#ifndef UART_H
#define	UART_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void UART_conf(void);
uint8_t Receive_Data(void);
void Send_Data(char X);
void Send_String(char* X);

#endif	/* XC_HEADER_TEMPLATE_H */

