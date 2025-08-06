/*
 * pwm.h
 *
 * Created: 29/7/2025 11:27:29
 *  Author: Santiago
 */ 


#ifndef PWM0_H_
#define PWM0_H_

#include <avr/io.h>
#include <stdint.h>

#define NO_INVERTING  1
#define INVERTING     0

void PWM0_init(void);
void PWM0_dca(uint8_t dc, uint8_t modo);
void PWM0_dcb(uint8_t dc, uint8_t modo);
void PWM0_dcsw(uint8_t dc);  // Nuevo PWM software en PB5

#endif /* PWM0_H_ */
