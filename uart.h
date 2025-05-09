#include <xc.h>
#include <pic18f4550.h>
#include "Config.h"

#ifndef UART_H
#define	UART_H

void Uart_Init(unsigned long baud);
__bit Uart_Available(void);
void Uart_Send_Char(char data);
void Uart_Send_String(char *str);
char Uart_Read(void);
void Uart_Read_String(char* buf, int s_buf);
void Uart_Read_Substring(char* cp, char* vc, int inc, int fn);
void Uart_Read_StringUntil(char stop_c, char* buf, int st_size);

#endif