/*
 * pwm.c
 *
 * Created: 29/7/2025 11:27:44
 *  Author: Santiago
 */ 

#include "pwm.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t duty_sw = 0;      // Duty software (0-100%)
volatile uint8_t counter_sw = 0;

// Inicialización de los 3 PWM
void PWM0_init(void)
{
	// -------- PWM HARDWARE (Timer1) ----------
	// Fast PWM 8 bits
	TCCR1A |= (1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13);

	// Prescaler 1024
	TCCR1B |= (1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS10);

	// Configurar pines OC1A (PB1) y OC1B (PB2) como salida
	DDRB |= (1<<PB1) | (1<<PB2);

	// -------- PWM SOFTWARE (PB5 con Timer2 en CTC) ----------
	DDRB |= (1<<PB5);      // PB5 salida
	PORTB &= ~(1<<PB5);    // Inicial en bajo

	// Timer2 -> CTC, prescaler 64, OCR2A = 18
	TCCR2A = (1<<WGM21);                            // Modo CTC (Clear Timer on Compare)
	TCCR2B = (1<<CS22) | (1<<CS20);                 // Prescaler 64: CS22=1, CS21=0, CS20=1
	OCR2A = 18;                                      // Compare match a 18

	TIMSK2 |= (1<<OCIE2A);                           // Habilitar interrupción Compare Match A

	sei();   // Habilitar interrupciones globales
}

// Canal A (OC1A - PB1)
void PWM0_dca(uint8_t dc, uint8_t modo)
{
	if (modo == NO_INVERTING)
	{
		TCCR1A |= (1<<COM1A1);
		TCCR1A &= ~(1<<COM1A0);
	}
	else // INVERTING
	{
		TCCR1A |= (1<<COM1A1);
		TCCR1A |= (1<<COM1A0);
	}
	OCR1A = (dc * 255) / 100;  // duty en %
}

// Canal B (OC1B - PB2)
void PWM0_dcb(uint8_t dc, uint8_t modo)
{
	if (modo == NO_INVERTING)
	{
		TCCR1A |= (1<<COM1B1);
		TCCR1A &= ~(1<<COM1B0);
	}
	else // INVERTING
	{
		TCCR1A |= (1<<COM1B1);
		TCCR1A |= (1<<COM1B0);
	}
	OCR1B = (dc * 255) / 100;  // duty en %
}

// Canal software (PB5)
void PWM0_dcsw(uint8_t dc)
{
	if (dc > 100) dc = 100;
	duty_sw = dc;
}

// ISR Timer2 Compare Match A -> PWM por software en PB5
ISR(TIMER2_COMPA_vect)
{
	counter_sw++;
	if (counter_sw < duty_sw)
	PORTB |= (1<<PB5);
	else
	PORTB &= ~(1<<PB5);

	if (counter_sw >= 100)
	counter_sw = 0;
}
