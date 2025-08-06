/*
 * uart.h
 *
 * Created: 30/7/2025 16:32:51
 *  Author: Santiago
 */ 

#ifndef UART_H
#define UART_H

#include <avr/io.h>

void UART_Init(void);
void UART_EnviarString(const char *cadena);
char UART_LeerChar(void);

#endif // UART_H