/*
 * adc.c
 *
 * Created: 29/7/2025 11:27:56
 *  Author: Santiago
 */ 

#include "adc.h"
#include <avr/interrupt.h>

void ADC_init(void)
{
	ADMUX = 0;               // Limpiar registro para evitar residuos
	ADMUX |= (1<<REFS0);     // AVcc como referencia (5v)
	ADMUX &= ~(1<<ADLAR);    

	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); // Prescaler 128 -> 125kHz
	ADCSRA |= (1<<ADEN);     // Habilitar ADC
	ADCSRA |= (1<<ADIE);     // Habilitar interrupción ADC
}

void ADC_start(uint8_t channel)
{
	channel &= 0x0F;
	ADMUX = (ADMUX & 0xF0) | channel;
	ADCSRA |= (1<<ADSC);     // Iniciar conversión
}

uint16_t ADC_read(uint8_t channel)
{
	ADC_start(channel);
	while(ADCSRA & (1<<ADSC));
	return ADC;
}