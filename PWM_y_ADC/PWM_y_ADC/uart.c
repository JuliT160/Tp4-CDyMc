/*
 * uart.c
 *
 * Created: 30/7/2025 16:33:03
 *  Author: Santiago
 */ 

#include "uart.h"

void UART_Init(void) {
	UBRR0H = 0;                         // 9600 bps @ 16 MHz
	UBRR0L = 103;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // TX + RX + Interrupcion RX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits, 1 bit de parada
}

void UART_EnviarString(const char *cadena) {
	while (*cadena) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *cadena++;
	}
}

char UART_LeerChar(void) {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}